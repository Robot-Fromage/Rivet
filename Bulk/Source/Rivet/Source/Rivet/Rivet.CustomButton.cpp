/**
*
*   Rivet
*__________________
*
* @file     Rivet.CustomButton.cpp
* @author   Clement Berthaud
* @brief    This file provides the definition for the RCustomButton class.
*/
#include "Rivet/Rivet.CustomButton.h"
#include <QPainter>
#include <QMouseEvent>

namespace  Rivet {
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default Values Defines
#define  DEFAULT_BG_COLOR                   QColor( 0, 0, 0, 0 )
#define  DEFAULT_HOVER_BG_COLOR             QColor( 255, 255, 255, 127 )
#define  DEFAULT_PRESSED_BG_COLOR           QColor( 0, 0, 0, 127 )
#define  DEFAULT_PRESSED_HOVERED_BG_COLOR   QColor( 0, 0, 0, 127 )
#define  DEFAULT_SHAPE                      eButtonIconShape::kNone
#define  DEFAULT_COLOR                      QColor( 255, 255, 255, 255 )
#define  DEFAULT_SIZE                       7
#define  DEFAULT_BACKGROUND                 eButtonBackgroundShape::kSquare


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
RCustomButton::~RCustomButton()
{
}


RCustomButton::RCustomButton( QWidget* iParent )
    : tSuperClass(              iParent                             )
    , mHovered(                 false                               )
    , mPressed(                 false                               )
    , mBgColor(                 DEFAULT_BG_COLOR                    )
    , mHoveredBgColor(          DEFAULT_HOVER_BG_COLOR              )
    , mPressedBgColor(          DEFAULT_PRESSED_BG_COLOR            )
    , mPressedHoveredBgColor(   DEFAULT_PRESSED_HOVERED_BG_COLOR    )
    , mColor(                   DEFAULT_COLOR                       )
    , mHoveredColor(            DEFAULT_COLOR                       )
    , mPressedColor(            DEFAULT_COLOR                       )
    , mPressedHoveredColor(     DEFAULT_COLOR                       )
    , mIconShape(               DEFAULT_SHAPE                       )
    , mBackgroundShape(         DEFAULT_BACKGROUND                  )
    , mIconSize(                DEFAULT_SIZE                        )
{
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Public Style Setup Accessors
void
RCustomButton::SetBackgroundColor( const  QColor&  iColor )
{
    mBgColor = iColor;
    repaint();
}


void
RCustomButton::SetHoveredBackgroundColor( const  QColor&  iColor )
{
    mHoveredBgColor = iColor;
    repaint();
}


void
RCustomButton::SetPressedBackgroundColor( const  QColor&  iColor )
{
    mPressedBgColor = iColor;
    mPressedHoveredBgColor = iColor;
    repaint();
}


const  QColor&
RCustomButton::GetBackgroundColor()  const
{
    return  mBgColor;
}


const  QColor&
RCustomButton::GetHoveredBackgroundColor()  const
{
    return  mHoveredBgColor;
}


const  QColor&
RCustomButton::GetPressedBackgroundColor()  const
{
    return  mPressedBgColor;
}


void
RCustomButton::SetColor( const  QColor&  iColor )
{
    mColor = iColor;
    repaint();
}


void
RCustomButton::SetHoveredColor( const  QColor&  iColor )
{
    mHoveredColor = iColor;
    repaint();
}


void
RCustomButton::SetPressedColor( const  QColor&  iColor )
{
    mPressedColor = iColor;
    mPressedHoveredColor = iColor;
    repaint();
}


const  QColor&
RCustomButton::GetColor()  const
{
    return  mColor;
}


const  QColor&
RCustomButton::GetHoveredColor()  const
{
    return  mHoveredColor;
}


const  QColor&
RCustomButton::GetPressedColor()  const
{
    return  mPressedColor;
}


void
RCustomButton::SetIconShape( const  eButtonIconShape&  iShape )
{
    mIconShape = iShape;
    repaint();
}


const  RCustomButton::eButtonIconShape&
RCustomButton::GetShape()  const
{
    return  mIconShape;
}


void
RCustomButton::SetBackgroundShape( const  eButtonBackgroundShape&  iShape )
{
    mBackgroundShape = iShape;
}


const  RCustomButton::eButtonBackgroundShape&
RCustomButton::GetBackgroundShape()  const
{
    return  mBackgroundShape;
}


void
RCustomButton::SetIconSize( int iSize )
{
    mIconSize = iSize;
    repaint();
}


int
RCustomButton::GetIconSize()  const
{
    return  mIconSize;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Public State Accessors
bool
RCustomButton::IsHovered()
{
    return  mPressed;
}


bool
RCustomButton::IsPressed()
{
    return  mHovered;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
void
RCustomButton::enterEvent( QEvent* event )
{
    tSuperClass::enterEvent( event );

    mHovered = true;
    repaint();
}


void
RCustomButton::leaveEvent( QEvent* event )
{
    tSuperClass::leaveEvent( event );

    mHovered = false;
    repaint();
}


void
RCustomButton::mousePressEvent( QMouseEvent* event )
{
    tSuperClass::mousePressEvent( event );

    if( event->button() == Qt::LeftButton )
    {
        mPressed = true;
        repaint();
    }
}


void
RCustomButton::mouseReleaseEvent( QMouseEvent* event )
{
    tSuperClass::mouseReleaseEvent( event );

    mPressed = false;
    repaint();
}


void
RCustomButton::mouseDoubleClickEvent( QMouseEvent* event )
{
    emit  DoubleClicked();
}


void
RCustomButton::paintEvent( QPaintEvent* event )
{
    /*
    // [DEPRECATED] Apply style sheet drawing on this before all
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

    switch( mBackgroundShape )
    {
        case  eButtonBackgroundShape::kSquare :
        {
            painter.setRenderHint( QPainter::Antialiasing, false );
            painter.setBrush( currentBackgroundColor );
            painter.setPen( Qt::NoPen );
            painter.drawRect( QRect( QPoint(), size() ) );
            painter.setRenderHint( QPainter::Antialiasing, false );
            break;
        }

        case  eButtonBackgroundShape::kDisk :
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

    int s2 = mIconSize / 2;
    auto hs = size() / 2;
    QPoint center = QPoint( hs.width(), hs.height() );
    QPoint refPointA( center + QPoint( -s2, -s2 ) );
    QPoint refPointB( center + QPoint( +s2, -s2 ) );
    QPoint refPointC( center + QPoint( +s2, +s2 ) );
    QPoint refPointD( center + QPoint( -s2, +s2 ) );

    switch( mIconShape )
    {
        case eButtonIconShape::kNone :
        {
            // Default drawing ?
            break;
        }

        case eButtonIconShape::kClose :
        {
            painter.drawLine( refPointA, refPointC + QPoint( +1, +1 ) );
            painter.drawLine( refPointD, refPointB + QPoint( +1, -1 ) );
            break;
        }

        case eButtonIconShape::kMaximize :
        {
            painter.drawLine( refPointA, refPointB );
            painter.drawLine( refPointB, refPointC + QPoint( +1, +1 ) );
            painter.drawLine( refPointC + QPoint( +1, +1 ), refPointD );
            painter.drawLine( refPointD, refPointA );
            break;
        }

        case eButtonIconShape::kMinimize :
        {
            painter.drawLine( ( refPointA + refPointD ) / 2, ( refPointB + refPointC ) / 2 );
            break;
        }

        case eButtonIconShape::kLeftArrow :
        {
            painter.setRenderHint( QPainter::Antialiasing, true );
            QPainterPath bezierPath;
            bezierPath.moveTo( ( refPointA + refPointD ) / 2 );
            bezierPath.lineTo( refPointB );
            bezierPath.lineTo( refPointC );
            bezierPath.closeSubpath();
            // Disabled fill:
            //painter.setBrush( currentColor );
            painter.drawPath( bezierPath );
            break;
        }

        case eButtonIconShape::kRightArrow :
        {
            painter.setRenderHint( QPainter::Antialiasing, true );
            QPainterPath bezierPath;
            bezierPath.moveTo( ( refPointB + refPointC ) / 2 );
            bezierPath.lineTo( refPointD );
            bezierPath.lineTo( refPointA );
            bezierPath.closeSubpath();
            // Disabled fill:
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

