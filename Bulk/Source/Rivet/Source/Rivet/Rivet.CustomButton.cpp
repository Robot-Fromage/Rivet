/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.CustomButton.cpp
* 8-9-2018 21:04 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include "Rivet/Rivet.CustomButton.h"


#include <QPainter>
#include <QMouseEvent>


namespace  Rivet
{


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default Values Defines


#define  DEFAULT_BG_COLOR                   QColor( 0, 0, 0, 0 )
#define  DEFAULT_HOVER_BG_COLOR             QColor( 255, 255, 255, 127 )
#define  DEFAULT_PRESSED_BG_COLOR           QColor( 0, 0, 0, 127 )
#define  DEFAULT_PRESSED_HOVERED_BG_COLOR   QColor( 0, 0, 0, 127 )
#define  DEFAULT_SHAPE                      eShape::kNone
#define  DEFAULT_COLOR                      QColor( 255, 255, 255, 255 )
#define  DEFAULT_SIZE                       7
#define  DEFAULT_BACKGROUND                 eBackground::kSquare


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


CustomButton::~CustomButton()
{
}


CustomButton::CustomButton(QWidget *parent) :
    tSuperClass(                parent ),
    mHovered(               false ),
    mPressed(               false ),
    mBgColor(               DEFAULT_BG_COLOR ),
    mHoveredBgColor(        DEFAULT_HOVER_BG_COLOR ),
    mPressedBgColor(        DEFAULT_PRESSED_BG_COLOR ),
    mPressedHoveredBgColor( DEFAULT_PRESSED_HOVERED_BG_COLOR ),
    mColor(                 DEFAULT_COLOR ),
    mHoveredColor(          DEFAULT_COLOR ),
    mPressedColor(          DEFAULT_COLOR ),
    mPressedHoveredColor(   DEFAULT_COLOR ),
    mShape(                 DEFAULT_SHAPE ),
    mSize(                  DEFAULT_SIZE ),
    mBackground(            DEFAULT_BACKGROUND )
{
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Public Style Setup Accessors


void
CustomButton::SetBackgroundColor( const  QColor&  iColor )
{
    mBgColor = iColor;
    repaint();
}


void
CustomButton::SetHoveredBackgroundColor( const  QColor&  iColor )
{
    mHoveredBgColor = iColor;
    repaint();
}


void
CustomButton::SetPressedBackgroundColor( const  QColor&  iColor )
{
    mPressedBgColor = iColor;
    mPressedHoveredBgColor = iColor;
    repaint();
}


const  QColor&
CustomButton::GetBackgroundColor()  const
{
    return  mBgColor;
}


const  QColor&
CustomButton::GetHoveredBackgroundColor()  const
{
    return  mHoveredBgColor;
}


const  QColor&
CustomButton::GetPressedBackgroundColor()  const
{
    return  mPressedBgColor;
}


void
CustomButton::SetColor( const  QColor&  iColor )
{
    mColor = iColor;
    repaint();
}


void
CustomButton::SetHoveredColor( const  QColor&  iColor )
{
    mHoveredColor = iColor;
    repaint();
}


void
CustomButton::SetPressedColor( const  QColor&  iColor )
{
    mPressedColor = iColor;
    mPressedHoveredColor = iColor;
    repaint();
}


const  QColor&
CustomButton::GetColor()  const
{
    return  mColor;
}


const  QColor&
CustomButton::GetHoveredColor()  const
{
    return  mHoveredColor;
}


const  QColor&
CustomButton::GetPressedColor()  const
{
    return  mPressedColor;
}


void
CustomButton::SetShape( const  eShape&  iShape )
{
    mShape = iShape;
    repaint();
}


const  CustomButton::eShape&
CustomButton::GetShape()  const
{
    return  mShape;
}


void
CustomButton::SetBackgroundShape( const  eBackground&  iShape )
{
    mBackground = iShape;
}


const  CustomButton::eBackground&
CustomButton::GetBackgroundShape()  const
{
    return  mBackground;
}


void
CustomButton::SetSize( int iSize )
{
    mSize = iSize;
    repaint();
}


int
CustomButton::GetSize()  const
{
    return  mSize;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides


bool
CustomButton::IsHovered()
{
    return  mPressed;
}


bool
CustomButton::IsPressed()
{
    return  mHovered;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides


void
CustomButton::enterEvent( QEvent* event )
{
    tSuperClass::enterEvent( event );

    mHovered = true;
    repaint();
}


void
CustomButton::leaveEvent( QEvent* event )
{
    tSuperClass::leaveEvent( event );

    mHovered = false;
    repaint();
}


void
CustomButton::mousePressEvent( QMouseEvent* event )
{
    tSuperClass::mousePressEvent( event );

    if( event->button() == Qt::LeftButton )
    {
        mPressed = true;
        repaint();
    }
}


void
CustomButton::mouseReleaseEvent( QMouseEvent* event )
{
    tSuperClass::mouseReleaseEvent( event );

    mPressed = false;
    repaint();
}


void
CustomButton::mouseDoubleClickEvent( QMouseEvent* event )
{
    emit  DoubleClicked();
}


void
CustomButton::paintEvent( QPaintEvent* event )
{
    /*
    // Apply style sheet drawing on this before all
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    */

    // Then use custom painting
    QPainter painter(this);
    painter.setRenderHint( QPainter::Antialiasing, false );

    eButtonState flag = static_cast< eButtonState >( int( mHovered ) | ( int( mPressed ) << 1 ) );
    QColor currentBackgroundColor = QColor( 0, 0, 0, 0 );
    QColor currentColor = QColor( 255, 255, 255, 255 );

    switch( flag )
    {
        case eButtonState::kNone:
        {
            currentBackgroundColor = mBgColor;
            currentColor = mColor;
            break;
        } 

        case eButtonState::kHovered:
        {
            currentBackgroundColor = mHoveredBgColor;
            currentColor = mHoveredColor;
            break;
        }

        case eButtonState::kPressed:
        {
            currentBackgroundColor = mPressedBgColor;
            currentColor = mPressedColor;
            break;
        }

        case eButtonState::kPressedHovered:
        {
            currentBackgroundColor = mPressedHoveredBgColor;
            currentColor = mPressedHoveredColor;
            break;
        }

        default:
        {
            currentBackgroundColor = mBgColor;
            currentColor = mColor;
        }

    }

    switch( mBackground )
    {
        case  eBackground::kSquare :
        {
            painter.setRenderHint( QPainter::Antialiasing, false );
            painter.setBrush( currentBackgroundColor );
            painter.setPen( Qt::NoPen );
            painter.drawRect( QRect( QPoint(), size() ) );
            painter.setRenderHint( QPainter::Antialiasing, false );
            break;
        }

        case  eBackground::kDisk :
        {
            painter.setRenderHint( QPainter::Antialiasing, true );
            painter.setBrush( currentBackgroundColor );
            painter.setPen( Qt::NoPen );
            painter.drawEllipse( QRect( QPoint(), size() ) );
            painter.setRenderHint( QPainter::Antialiasing, false );
            break;
        }
    }

    painter.setBrush( Qt::NoBrush );
    painter.setPen( QPen( currentColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin ) );

    int s2 = mSize / 2;
    auto hs = size() / 2;
    QPoint center = QPoint( hs.width(), hs.height() );
    QPoint refPointA( center + QPoint( -s2, -s2 ) );
    QPoint refPointB( center + QPoint( +s2, -s2 ) );
    QPoint refPointC( center + QPoint( +s2, +s2 ) );
    QPoint refPointD( center + QPoint( -s2, +s2 ) );

    switch( mShape )
    {
        case eShape::kNone :
        {
            // Default drawing ?
            break;
        }

        case eShape::kClose :
        {
            painter.drawLine( refPointA, refPointC + QPoint( +1, +1 ) );
            painter.drawLine( refPointD, refPointB + QPoint( +1, -1 ) );
            break;
        }

        case eShape::kMaximize :
        {
            painter.drawLine( refPointA, refPointB );
            painter.drawLine( refPointB, refPointC + QPoint( +1, +1 ) );
            painter.drawLine( refPointC + QPoint( +1, +1 ), refPointD );
            painter.drawLine( refPointD, refPointA );
            break;
        }

        case eShape::kMinimize :
        {
            painter.drawLine( ( refPointA + refPointD ) / 2, ( refPointB + refPointC ) / 2 );
            break;
        }

        case eShape::kLeftArrow :
        {
            painter.setRenderHint( QPainter::Antialiasing, true );
            QPainterPath bezierPath;
            bezierPath.moveTo( ( refPointA + refPointD ) / 2 );
            bezierPath.lineTo( refPointB );
            bezierPath.lineTo( refPointC );
            bezierPath.closeSubpath();
            //painter.setBrush( currentColor );
            painter.drawPath( bezierPath );
            break;
        }

        case eShape::kRightArrow :
        {
            painter.setRenderHint( QPainter::Antialiasing, true );
            QPainterPath bezierPath;
            bezierPath.moveTo( ( refPointB + refPointC ) / 2 );
            bezierPath.lineTo( refPointD );
            bezierPath.lineTo( refPointA );
            bezierPath.closeSubpath();
            //painter.setBrush( currentColor );
            painter.drawPath( bezierPath );
            break;
        }

        default:
        {
            // Default drawing ?
        }
    }
}


} // namespace  Rivet

