/**
*
*   Rivet
*__________________
*
* @file     Rivet.CustomMainWindow.cpp
* @author   Clement Berthaud
* @brief    This file provides the definition for the RCustomWindow class.
*/
#include "Rivet/Rivet.CustomMainWindow.h"

namespace  Rivet {
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default values defines
#define  DEFAULT_MINIMUM_WIDTH 200


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
RCustomMainWindow::~RCustomMainWindow()
{
    tSelf::Destroy();
}


RCustomMainWindow::RCustomMainWindow( QWidget* iParent )
    : tSuperClass(      iParent )
    , mCaptionWidget(   nullptr )
    , mCenterWidget(    nullptr )
{
    tSelf::Init();
    tSelf::Build();
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Center Caption API
RCustomCaption*
RCustomMainWindow::CaptionWidget()  const
{
    return  mCaptionWidget;
}


QWidget*
RCustomMainWindow::CenterWidget()  const
{
    return  mCenterWidget;
}


void
RCustomMainWindow::SetCaptionWidget( RCustomCaption* iCaptionWidget )
{
    if( mCaptionWidget )
    {
        // this owns mCaptionWidget, hence has responsibility to delete
        delete  mCaptionWidget;
        mCaptionWidget = nullptr;
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


void
RCustomMainWindow::SetCenterWidget( QWidget* iCenterWidget )
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
RCustomMainWindow::Recompose()
{
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//--------------------------------- Protected Non-Client OS behaviour handling overrides
bool
RCustomMainWindow::NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    if( !tSuperClass::NCHitCaption( iRect, iBorderWidth, iX, iY ) )
        return  false;

    QPoint local = mapFromGlobal( QPoint( iX, iY ) );
    if( mCaptionWidget )
        return  mCaptionWidget->HitEmptySpace( local.x(), local.y() );
    else
        return  true;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt events override
void
RCustomMainWindow::resizeEvent( QResizeEvent*  event )
{
    // Very important !
    tSuperClass::resizeEvent( event );

    // Recompute internal geometry
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------- Private Qt Slots
void
RCustomMainWindow::ProcessCloseClicked()
{
    close();
}

void
RCustomMainWindow::ProcessMaximizeClicked()
{
    const auto state = windowState();
    if( state & Qt::WindowMaximized )
    {
        Restore();
    }
    else
    {
        showMaximized();
    }
}


void
RCustomMainWindow::ProcessMinimizeClicked()
{
    showMinimized();
}


//--------------------------------------------------------------------------------------
//--------------------------------- Protected Non-Client OS behaviour handling overrides
void
RCustomMainWindow::Init()
{
    mCaptionWidget  = nullptr;
    mCenterWidget   = nullptr;
}


void
RCustomMainWindow::Build()
{
    setMinimumWidth( DEFAULT_MINIMUM_WIDTH );
}


void
RCustomMainWindow::Compose()
{
    if( mCaptionWidget )
        mCaptionWidget->setGeometry( CaptionGeometry() );

    if( mCenterWidget )
        mCenterWidget->setGeometry( ContentsGeometry() );
}


void
RCustomMainWindow::Destroy()
{
    if( mCaptionWidget )
    {
        // This is owner of mCaptionWidget, responsibility to delete
        delete  mCaptionWidget;
        mCaptionWidget = nullptr;
    }

    if( mCenterWidget )
    {
        // This is owner of mCaptionWidget, responsibility to delete
        delete  mCenterWidget;
        mCenterWidget = nullptr;
    }
}


} // namespace  Rivet

