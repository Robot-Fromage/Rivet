/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.CustomMainWindow.cpp
*
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include "Rivet/Rivet.CustomMainWindow.h"


#include <QApplication>


namespace  Rivet
{


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default values defines


#define  DEFAULT_MINIMUM_WIDTH 200


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


CustomMainWindow::~CustomMainWindow()
{
    tSelf::Destroy();
}


CustomMainWindow::CustomMainWindow( QWidget *parent ) :
    tSuperClass( parent ),
    mCaptionWidget( NULL ),
    mCenterWidget( NULL )
{
    tSelf::Init();
    tSelf::Build();
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- Caption API


CustomCaption*
CustomMainWindow::CaptionWidget()
{
    return  mCaptionWidget;
}


void
CustomMainWindow::SetCaptionWidget( CustomCaption* iCaptionWidget )
{
    if( mCaptionWidget )
    {
        // This is owner of mCaptionWidget, responsibility to delete
        delete  mCaptionWidget;
        mCaptionWidget = NULL;
    }

    if( iCaptionWidget )
    {
        mCaptionWidget = iCaptionWidget;
        iCaptionWidget->setParent( this );
        iCaptionWidget->show();
        QObject::connect( mCaptionWidget, SIGNAL( CloseClicked() ), this, SLOT( ProcessCloseClicked() ) );
        QObject::connect( mCaptionWidget, SIGNAL( MaximizeClicked() ), this, SLOT( ProcessMaximizeClicked() ) );
        QObject::connect( mCaptionWidget, SIGNAL( MinimizeClicked() ), this, SLOT( ProcessMinimizeClicked() ) );
    }
}


QWidget*
CustomMainWindow::CenterWidget()
{
    return  mCenterWidget;
}


void
CustomMainWindow::SetCenterWidget( QWidget* iCenterWidget )
{
    if( mCenterWidget )
    {
        delete  mCenterWidget;
        mCenterWidget = NULL;
    }

    if( iCenterWidget )
    {
        mCenterWidget = iCenterWidget;
        iCenterWidget->setParent( this );
        iCenterWidget->show();
    }
}


void
CustomMainWindow::Recompose()
{
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//--------------------------------- Protected Non-Client OS behaviour handling overrides


bool
CustomMainWindow::NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    bool  eligible = tSuperClass::NCHitCaption( iRect, iBorderWidth, iX, iY );
    QPoint local = mapFromGlobal( QPoint( iX, iY ) );

    if(!eligible)
        return  false;

    if( mCaptionWidget )
    {
        return  mCaptionWidget->HitEmptySpace( local.x(), local.y() );
    }
    else
    {
        return  true;
    }
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt events override


void
CustomMainWindow::resizeEvent( QResizeEvent*  event )
{
    // Very important !
    tSuperClass::resizeEvent( event );

    // Recompute internal geometry
    tSelf::Compose();
}



//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------- Private Qt Slots


void
CustomMainWindow::ProcessCloseClicked()
{
    close();
}

void
CustomMainWindow::ProcessMaximizeClicked()
{
    const auto state = windowState();
    if(state & Qt::WindowMaximized)
    {
        Restore();
    }
    else
    {
        showMaximized();
    }
}


void
CustomMainWindow::ProcessMinimizeClicked()
{
    showMinimized();
}


//--------------------------------------------------------------------------------------
//--------------------------------- Protected Non-Client OS behaviour handling overrides


void
CustomMainWindow::Init()
{
    // Not much there ATM but keep for consistency with the rest of the API GUI convention in my other Libs.
    mCaptionWidget = NULL;
    mCenterWidget = NULL;
}


void
CustomMainWindow::Build()
{
    setMinimumWidth( DEFAULT_MINIMUM_WIDTH );
}


void
CustomMainWindow::Compose()
{
    if( mCaptionWidget )
    {
        mCaptionWidget->setGeometry( CaptionGeometry() );
    }

    if( mCenterWidget )
    {
        mCenterWidget->setGeometry( ContentsGeometry() );
    }
}


void
CustomMainWindow::Destroy()
{
    if( mCaptionWidget )
    {
        // This is owner of mCaptionWidget, responsibility to delete
        delete  mCaptionWidget;
        mCaptionWidget = NULL;
    }

    if( mCenterWidget )
    {
        // This is owner of mCaptionWidget, responsibility to delete
        delete  mCenterWidget;
        mCenterWidget = NULL;
    }
}


} // namespace  Rivet

