/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.DefaultCaption.cpp
* 9-9-2018 18:57 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include "Rivet/Rivet.DefaultCaption.h"


#include <QPainter>


namespace  Rivet
{


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default values defines


#define  DEFAULT_ACTIVE_BACKGROUND_COLOR QColor( 0, 0, 0, 127 )
#define  DEFAULT_INACTIVE_BACKGROUND_COLOR QColor( 0, 0, 0, 50 )


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


DefaultCaption::~DefaultCaption()
{
    Destroy();
}

DefaultCaption::DefaultCaption( QWidget* parent ) :
    tSuperClass( parent ),
    mCloseButton(       NULL ),
    mMaximizeButton(    NULL ),
    mMinimizeButton(    NULL ),
    mActiveBackgroundColor( DEFAULT_ACTIVE_BACKGROUND_COLOR ),
    mInactiveBackgroundColor( DEFAULT_INACTIVE_BACKGROUND_COLOR )
{
    Init();
    Build();
    Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Geometry public API


QRect
DefaultCaption::ButtonsGeometry()
{
    return  QRect( mMinimizeButton->x(), 0, width() - mMinimizeButton->x(), height() );
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------- Public customization utilities


void
DefaultCaption::ActiveBackgroundColor( const  QColor& iColor )
{
    mActiveBackgroundColor = iColor;
}


const  QColor&
DefaultCaption::ActiveBackgroundColor()
{
    return  mActiveBackgroundColor;
}

void
DefaultCaption::InactiveBackgroundColor( const  QColor& iColor )
{
    mInactiveBackgroundColor = iColor;
}


const  QColor&
DefaultCaption::InactiveBackgroundColor()
{
    return  mInactiveBackgroundColor;
}


void
DefaultCaption::SetBlackControls()
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
DefaultCaption::SetWhiteControls()
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
DefaultCaption::paintEvent(  QPaintEvent*    event )
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
DefaultCaption::resizeEvent( QResizeEvent *event )
{
    QWidget::resizeEvent(event);
    Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities


void
DefaultCaption::Init()
{
    if( !mCloseButton )     mCloseButton    = new  RCustomButton( this );
    if( !mMaximizeButton )  mMaximizeButton = new  RCustomButton( this );
    if( !mMinimizeButton )  mMinimizeButton = new  RCustomButton( this );
}


void
DefaultCaption::Build()
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
DefaultCaption::Compose()
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
DefaultCaption::Destroy()
{
    delete  mCloseButton;
    delete  mMaximizeButton;
    delete  mMinimizeButton;

    mCloseButton = 0;
    mMaximizeButton = 0;
    mMinimizeButton = 0;
}


} // namespace  Rivet

