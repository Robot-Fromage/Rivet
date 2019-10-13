/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.__private__.DockingManager.cpp
* 3-10-2018 20:38 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include "Rivet/__private__/Rivet.__private__.DockingManager.h"


#include "Rivet/Rivet.Tab.h"
#include "Rivet/Rivet.TabArea.h"
#include "Rivet/__private__/Rivet.__private__.GeometryUtils.h"
#include "Rivet/__private__/Rivet.__private__.WinExtras.h"

#include <QApplication>
#include <QShortcut>
#include <QEvent>
#include <QMouseEvent>


#include <assert.h>


////////////////////////////////////////////////////////////////////////////////////////
////                                STATIC TOOLS                                    ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//------------------------------------------------- Static functions & tools for sorting


struct  FZOrderingPair
{
    int zOrder;
    ::Rivet::RTabArea* area;
};

// Struct for ordering and selecting area while dragging
struct FElligibleArea
{
    ::Rivet::RTabArea*  mArea;
    QRegion   mRegion;
};

static
bool
SortZ( const  FZOrderingPair& iA, const  FZOrderingPair& iB )
{
    return  iA.zOrder < iB.zOrder;
}


namespace  Rivet
{
namespace  __private__
{


////////////////////////////////////////////////////////////////////////////////////////
////                            PRIVATE CONSTRUCTION                                ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


FDockingManager::~FDockingManager()
{
}


FDockingManager::FDockingManager() :
    mCurrentDraggingTab( NULL ),
    mCurrentTargetArea( NULL ),
    mLastLiftedFrom( NULL )
{
}


////////////////////////////////////////////////////////////////////////////////////////
////                            PUBLIC SINGLETON API                                ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Singleton Accessor


// static
FDockingManager*
FDockingManager::DockingManager()
{
    static  FDockingManager*  sgDockingManager = 0;

    if( !sgDockingManager )
        sgDockingManager = new  FDockingManager();

    return  sgDockingManager;
}


////////////////////////////////////////////////////////////////////////////////////////
////                                INFO API                                        ////
////////////////////////////////////////////////////////////////////////////////////////


RTab*
FDockingManager::CurrentDraggingTab()  const
{
    return  mCurrentDraggingTab;
}


RTabArea*
FDockingManager::CurrentTargetArea()  const
{
    return  mCurrentTargetArea;
}


void
FDockingManager::SetLastLiftedFrom( RTabArea* iValue )
{
    mLastLiftedFrom = iValue;
}


RTabArea*
FDockingManager::GetLastLiftedFrom()  const
{
    return  mLastLiftedFrom;
}

////////////////////////////////////////////////////////////////////////////////////////
////                            REGISTER API                                        ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//---------------------------------------------- Public Register API for Tabs & TabAreas


void
FDockingManager::RegisterTabArea( RTabArea* iTabArea )
{
    mTabAreaList.append( iTabArea );
}


void
FDockingManager::UnregisterTabArea( RTabArea* iTabArea )
{
    mTabAreaList.removeAll( iTabArea );
}


void
FDockingManager::RegisterTab( RTab* iTab )
{
    InitConnectionsForTab( iTab );
}


void
FDockingManager::UnregisterTab( RTab* iTab )
{
    DestroyConnectionsForTab( iTab );
}


////////////////////////////////////////////////////////////////////////////////////////
////                            PRIVATE SIGNAL SLOTS API                            ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------- Docking Interface Slots


void
FDockingManager::TabLifted( RTab* iTab )
{
    // Processing directly after the signal was emitted

    // Hook the dragging tab
    mCurrentDraggingTab = iTab;
    mCurrentDraggingTab->installEventFilter( this );

    // Make it Indie
    iTab->setParent(0);
    iTab->setWindowFlags( Qt::FramelessWindowHint | Qt::SubWindow );
    iTab->show();
    iTab->raise();
}


void
FDockingManager::TabDropped( RTab* iTab )
{
    assert( iTab == mCurrentDraggingTab );
    mCurrentDraggingTab->removeEventFilter( this );

    if( !mCurrentTargetArea )
    {
        //emit  TabDroppedOutisde( mCurrentDraggingTab );
        auto fctptr = mCurrentDraggingTab->GetOnTabDroppedOutCB();
        if( fctptr )
            fctptr( mCurrentDraggingTab, mLastLiftedFrom );
    }

    //mCurrentTargetArea = NULL;
    //mCurrentDraggingTab = NULL;
}


////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE API                                     ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Qt Events overrides


bool
FDockingManager::eventFilter( QObject* obj, QEvent* event )
{
    // We process only mouse events of the current dragging tab.
    {
        RTab* tab = dynamic_cast< RTab* >( obj );

        if( !tab )
            // return false means process the event normally instead
            return  false;

        assert( tab->Dragging() );
        assert( tab == mCurrentDraggingTab );
    }

    // This part allows us to cancel the dragging in case an unexpected event occured
    {
        auto eType = event->type();
        if( eType == QEvent::FocusOut || eType == QEvent::Leave || eType == QEvent::HoverLeave || eType == QEvent::ContextMenu || eType == QEvent::Drop || eType == QEvent::NonClientAreaMouseButtonRelease )
        {
            mCurrentDraggingTab->FinishDrag(); // cancel drag will emit the necessary signals to remove the event filter.
            event->ignore(); // explicit ignore, not sure about the effect of this.
            return  true; // return  true cancels any further event propagation & computation.
        }
    }

    // Actual eventFilter behaviour starts here
    // We process only mouse events ( most likely move events )
    QMouseEvent* mouseEvent = dynamic_cast< QMouseEvent* >( event );
    if( !mouseEvent )
        return  false; // return false means process the event normally instead

    // Positionning
    // Position of the mouse at the time of event, see also QCursor::pos()
    QPoint cpos = mouseEvent->globalPos();
    QRect tabRec = mCurrentDraggingTab->geometry();

    // Mouse Snapping when close to the Area that has been targeted.
    if( mCurrentTargetArea )
    {
        QPoint globalSnap = mCurrentTargetArea->mapToGlobal(QPoint(0,0));
        int h2 = mCurrentTargetArea->height() / 2;
        int threshold = 10;
        if( abs( globalSnap.y() + h2 - mouseEvent->globalPos().y() ) < threshold )
            mCurrentDraggingTab->move( cpos.x() - mCurrentDraggingTab->DragShift().x(), globalSnap.y());
        else
            mCurrentDraggingTab->move( cpos - mCurrentDraggingTab->DragShift());
    }
    else
    {
        mCurrentDraggingTab->move( cpos - mCurrentDraggingTab->DragShift());
    }

    // Selecting target area
    RTabArea* resultArea = NULL;

    QVector< FElligibleArea > elligibleVector;
    for( RTabArea* area : mTabAreaList )
    {
        // Reset tabAreas hooks before processing the new one
        // This is safe to do even if there is none
        mCurrentDraggingTab->removeEventFilter( area );
        QObject::disconnect( mCurrentDraggingTab, SIGNAL( Dropped( RTab* ) ), area, SLOT( ForeignTabDropped( RTab* ) ) );

        // Computing global Region for ech tabArea
        /*
        QRegion region = area->visibleRegion().translated( area->mapToGlobal( QPoint( 0, 0 ) ) );
        QRect debug = region.boundingRect();
        QRect debug2 = area->geometry();
        */

        QRect region = MapRectToGlobal( area->parentWidget(), area->geometry() );
        // If there is no intersection that means the area is not elligible
        // It can mean the area lost track of the tab so we proceed to a reorder
        if( !region.intersects( tabRec ) )
        {
            continue;
        }

        // If we arrive here, the tabArea is elligible so we push it for further processing
        elligibleVector.push_back( FElligibleArea( { area, region } ) );
    }

    switch( elligibleVector.count() )
    {
        case 0:
        {
            resultArea = NULL;
            break;
        }

        case 1:
        {
            resultArea = elligibleVector[0].mArea;
            break;
        }

        default:
        {
            // The current dragging tab is overlapping more than one area
            QVector< FElligibleArea > overlappingCursorSelection;
            bool therAreOverlaping = false;
            for( FElligibleArea m  : elligibleVector )
            {
                // We chose the ones that contains the mouse cursor position
                if( m.mRegion.contains( cpos ) )
                {
                    overlappingCursorSelection.append( m );
                    therAreOverlaping = true;
                }
            }

            if( therAreOverlaping )
            {
                // If there is at least one region that contains the cursor position, we select the topmost in overlappingCursorSelection
                QVector< FZOrderingPair > orderingVector;
                for( FElligibleArea m  : overlappingCursorSelection )
                    orderingVector.append( FZOrderingPair{ GetZOrder( (HWND)m.mArea->topLevelWidget()->winId() ), m.mArea } );

                qSort( orderingVector.begin(), orderingVector.end(), SortZ );
                resultArea = orderingVector[0].area;
            }
            else
            {
                // Otherwise: overlapping more than one area but cursor is not in any of them, we select the topmost in elligible
                QVector< FZOrderingPair > orderingVector;
                for( FElligibleArea m  : elligibleVector )
                    orderingVector.append( FZOrderingPair{ GetZOrder( (HWND)m.mArea->topLevelWidget()->winId() ), m.mArea } );
                qSort( orderingVector.begin(), orderingVector.end(), SortZ );
                resultArea = orderingVector[0].area;
            }

        } // !default
    } // !switch

    // Reject if not tag
    if( resultArea && resultArea->GetTag() != mCurrentDraggingTab->GetTag() )
        return  false;

    // Reorder leaving target area
    if( resultArea != mCurrentTargetArea && mCurrentTargetArea != NULL )
        mCurrentTargetArea->Recompose();

    // Can be set to NULL
    mCurrentTargetArea = resultArea;

    if( resultArea )
    {
        mCurrentDraggingTab->installEventFilter( resultArea );
        QObject::connect( mCurrentDraggingTab, SIGNAL( Dropped( RTab* ) ), resultArea, SLOT( ForeignTabDropped( RTab* ) ) );
        resultArea->SetCandidateTab( mCurrentDraggingTab );
    }

    for( RTabArea* area : mTabAreaList )
        if( area != resultArea )
            area->SetCandidateTab( NULL );

    return  false;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Private Connection Interface


void
FDockingManager::InitConnectionsForTab( RTab* iTab )
{
    QObject::connect( iTab, SIGNAL( Lifted( RTab* ) ), this, SLOT( TabLifted( RTab* ) ) );
    QObject::connect( iTab, SIGNAL( Dropped( RTab* ) ), this, SLOT( TabDropped( RTab* ) ) );
}


void
FDockingManager::DestroyConnectionsForTab( RTab* iTab )
{
    QObject::disconnect( iTab, SIGNAL( Lifted( RTab* ) ), this, SLOT( TabLifted( RTab* ) ) );
    QObject::disconnect( iTab, SIGNAL( Dropped( RTab* ) ), this, SLOT( TabDropped( RTab* ) ) );
}


} // namespace  __private__


//--------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////
////                            PUBLIC SINGLETON API                                ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//---------------------------------------------- External Conveniency Singleton Accessor


::Rivet::__private__::FDockingManager*
DockingManager()
{
    return  ::Rivet::__private__::FDockingManager::DockingManager();
}


} // namespace  Rivet

