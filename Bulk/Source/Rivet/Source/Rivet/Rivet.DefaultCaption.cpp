/**
*
*   Rivet
*__________________
*
* @file     Rivet.DefaultCaption.cpp
* @author   Clement Berthaud
* @brief    This file provides the definition for the RDefaultCaption class.
*/
#include "Rivet/Rivet.DefaultCaption.h"
#include <QPainter>

namespace  Rivet {
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default values defines
#define     DEFAULT_ACTIVE_BACKGROUND_COLOR     QColor( 0, 0, 0, 127 )
#define     DEFAULT_INACTIVE_BACKGROUND_COLOR   QColor( 0, 0, 0, 50 )


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


RDefaultCaption::~RDefaultCaption()
{
    Destroy();
}

RDefaultCaption::RDefaultCaption( QWidget* iParent )
    : tSuperClass(              iParent                             )
    , mCloseButton(             nullptr                             )
    , mMaximizeButton(          nullptr                             )
    , mMinimizeButton(          nullptr                             )
    , mActiveBackgroundColor(   DEFAULT_ACTIVE_BACKGROUND_COLOR     )
    , mInactiveBackgroundColor( DEFAULT_INACTIVE_BACKGROUND_COLOR   )
{
    Init();
    Build();
    Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Public Geometry API
QRect
RDefaultCaption::ButtonsGeometry()
{
    return  QRect( mMinimizeButton->x(), 0, width() - mMinimizeButton->x(), height() );
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------- Public customization utilities
void
RDefaultCaption::ActiveBackgroundColor( const  QColor& iColor )
{
    mActiveBackgroundColor = iColor;
}


const  QColor&
RDefaultCaption::ActiveBackgroundColor()
{
    return  mActiveBackgroundColor;
}

void
RDefaultCaption::InactiveBackgroundColor( const  QColor& iColor )
{
    mInactiveBackgroundColor = iColor;
}


const  QColor&
RDefaultCaption::InactiveBackgroundColor()
{
    return  mInactiveBackgroundColor;
}


void
RDefaultCaption::SetBlackControls()
{
    QColor black = Qt::black;
    mCloseButton->SetColor( black );
    mCloseButton->SetHoveredColor( black );
    mCloseButton->SetPressedColor( black );

    mMaximizeButton->SetColor( black );
    mMaximizeButton->SetHoveredColor( black );
    mMaximizeButton->SetPressedColor( black );

    mMinimizeButton->SetColor( black );
    mMinimizeButton->SetHoveredColor( black );
    mMinimizeButton->SetPressedColor( black );
}


void
RDefaultCaption::SetWhiteControls()
{
    QColor white = Qt::white;
    mCloseButton->SetColor( white );
    mCloseButton->SetHoveredColor( white );
    mCloseButton->SetPressedColor( white );

    mMaximizeButton->SetColor( white );
    mMaximizeButton->SetHoveredColor( white );
    mMaximizeButton->SetPressedColor( white );

    mMinimizeButton->SetColor( white );
    mMinimizeButton->SetHoveredColor( white );
    mMinimizeButton->SetPressedColor( white );
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
void
RDefaultCaption::paintEvent(  QPaintEvent*    event )
{
    QPainter painter(this);
    painter.setRenderHint( QPainter::Antialiasing, false );
    QColor  bgColor = topLevelWidget()->isActiveWindow() ? mActiveBackgroundColor : mInactiveBackgroundColor;

    int w = width();
    int h = height();
    painter.setBrush( bgColor );
    painter.setPen( Qt::NoPen );
    painter.drawRect( 0, 0, w, h );
}


void
RDefaultCaption::resizeEvent( QResizeEvent *event )
{
    QWidget::resizeEvent(event);
    Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities
void
RDefaultCaption::Init()
{
    if( !mCloseButton )     mCloseButton    = new  RCustomButton( this );
    if( !mMaximizeButton )  mMaximizeButton = new  RCustomButton( this );
    if( !mMinimizeButton )  mMinimizeButton = new  RCustomButton( this );
}


void
RDefaultCaption::Build()
{
    mCloseButton->SetIconShape( RCustomButton::eButtonIconShape::kClose );
    mMaximizeButton->SetIconShape( RCustomButton::eButtonIconShape::kMaximize );
    mMinimizeButton->SetIconShape( RCustomButton::eButtonIconShape::kMinimize );

    mCloseButton->SetHoveredBackgroundColor( QColor( 230, 20, 35 ) );

    QObject::connect( mCloseButton, SIGNAL( clicked() ), this, SLOT( ProcessCloseClicked() ) );
    QObject::connect( mMaximizeButton, SIGNAL( clicked() ), this, SLOT( ProcessMaximizeClicked() ) );
    QObject::connect( mMinimizeButton, SIGNAL( clicked() ), this, SLOT( ProcessMinimizeClicked() ) );
}


void
RDefaultCaption::Compose()
{
    int w = width();
    int h = height();

    float buttonRatio   = 1.5f;
    int buttonHeight    = h;
    int buttonWidth     = buttonHeight * buttonRatio;

    QSize buttonSize( buttonWidth, h );
    mCloseButton->resize( buttonSize );
    mMaximizeButton->resize( buttonSize );
    mMinimizeButton->resize( buttonSize );

    int n = 0;
    mCloseButton->move(     w - buttonWidth * ++n, 0 );
    mMaximizeButton->move(  w - buttonWidth * ++n, 0 );
    mMinimizeButton->move(  w - buttonWidth * ++n, 0 );
}


void
RDefaultCaption::Destroy()
{
    delete  mCloseButton;
    delete  mMaximizeButton;
    delete  mMinimizeButton;

    mCloseButton = 0;
    mMaximizeButton = 0;
    mMinimizeButton = 0;
}


} // namespace  Rivet

