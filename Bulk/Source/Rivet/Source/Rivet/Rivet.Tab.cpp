/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.Tab.cpp
* 24-10-2018 19:16 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include "Rivet/Rivet.Tab.h"


#include "Rivet/Rivet.CustomButton.h"
#include "Rivet/__private__/Rivet.__private__.DockingManager.h"


#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default values defines


#define  DEFAULT_TAB_STYLE                  eTabShape::kBezier
#define  DEFAULT_BASE_COLOR                 QColor( 53, 53, 53, 255 )
#define  DEFAULT_FADE_COLOR                 QColor( 45, 45, 45, 127 )
#define  DEFAULT_TEXT_COLOR                 QColor( 255, 255, 255, 255 )
#define  DEFAULT_DROP_SHADOW_ALPHA          100
#define  DEFAULT_DROP_SHADOW_COLOR          QColor( 0, 0, 0, DEFAULT_DROP_SHADOW_ALPHA )
#define  DEFAULT_TITLE_TEXT                 "Untitled"
#define  DEFAULT_SLOPE_PADDING              20
#define  DEFAULT_TAB_ANIMATION_DURATION     100


namespace  Rivet
{


////////////////////////////////////////////////////////////////////////////////////////
////                                PUBLIC API                                      ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


Tab::~Tab()
{
    StopAnimatedMovement();
    Unregister();
    Destroy();
}


Tab::Tab( QWidget* parent ) :
    tSuperClass( parent ),
    mTitleLabel(                    NULL ),
    mCloseButton(                   NULL ),
    mInvisibleBackgroundRect(       NULL ),

    mTitleText(                     DEFAULT_TITLE_TEXT ),

    mDragShift(                     QPoint() ),
    mDragging(                      false ),
    mDragEnabled(                   false ),

    mAnimation(                     NULL ),
    mAnimationTargetGeometry(       QRect() ),

    mTabStyle(                      DEFAULT_TAB_STYLE ),
    mBaseColor(                     DEFAULT_BASE_COLOR ),
    mFadeColor(                     DEFAULT_FADE_COLOR ),
    mSlopePadding(                  DEFAULT_SLOPE_PADDING ),
    mTitleDropShadowEffect(         NULL ),
    mGlobalDropShadowEffect(        NULL ),
    mTextColor(                     DEFAULT_TEXT_COLOR ),
    mTitleDropShadowColor(          DEFAULT_DROP_SHADOW_COLOR ),

    mHovered(                       false ),
    mPressed(                       false ),
    mActive(                        false ),

    mLiftable( true ),
    mClosable( true ),
    mTag( "" ),

    mOnTabDroppedOutCB( NULL ),
    mLinkWidget( NULL )
{
    Init();
    Build();
    Compose();
    Register();
}


//--------------------------------------------------------------------------------------
//---------------------------------------------------------- Docking Interface Accessors


bool
Tab::Docked()  const
{
    // If undocked, that means top level, hence parentWidget is NULL
    return  bool( parentWidget() );
}


bool
Tab::Dragging()  const
{
    return  mDragging;
}


const  QPoint&
Tab::DragShift()  const
{
    return  mDragShift;
}


void
Tab::FinishDrag()
{
    // Forbid call if not actually dragging
    if( !mDragging )
        return;

    // Drop mouse tracking
    setMouseTracking( false );
    // Wait for next press event before enabling
    mDragEnabled = false;
    // Reset dragging flag
    mDragging = false;
    // Emit dropped so that tabarea and docking manager handle the external logic
    emit  Dropped(this);
    // Reset state flags
    mHovered = false;
    mPressed = false;
    // Set Active once dropped
    SetActive();
    // Trigger invalid repaint
    repaint();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Animation Interface


void
Tab::SetAnimatedMovement( const  QPoint& iDest )
{
    StopAnimatedMovement();

    // Animation enabled only if docked
    if( Docked() && !mAnimation )
    {
        mAnimation = new QPropertyAnimation( this, "geometry" );
        mAnimation->setDuration(    DEFAULT_TAB_ANIMATION_DURATION );
        mAnimation->setEasingCurve( QEasingCurve::OutCubic );
        QRect oldRect( pos(), size() );
        QRect newRect( iDest, size() );
        mAnimation->setStartValue(  oldRect );
        mAnimation->setEndValue(    newRect );
        mAnimation->start();
        mAnimationTargetGeometry = newRect;
    }
}


void
Tab::StopAnimatedMovement()
{
    mAnimationTargetGeometry = geometry();

    if( mAnimation )
    {
        mAnimation->stop();
        delete  mAnimation;
        mAnimation = 0;
    }
}


const  QRect&
Tab::TargetGeometry()  const
{
    return  mAnimationTargetGeometry;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------- Tab ColorStyle Interface Accessors


void
Tab::SetTitle( const  QString&  iTitle )
{
    // Set Text to member
    mTitleText = iTitle;
    // Check for Ellipsis
    QFontMetrics metrix( mTitleLabel->font() );
    int labelWidth      = mTitleLabel->width() - 2;
    QString clippedText = metrix.elidedText( mTitleText, Qt::ElideRight, labelWidth );
    // Set ellided Text to actual title label
    mTitleLabel->setText(clippedText);
}


void
Tab::SetTabShape( eTabShape iTabStyle )
{
    mTabStyle = iTabStyle;
    repaint();
}


void
Tab::SetColor( const  QColor&  iColor )
{
    mBaseColor = iColor;

    int l = mBaseColor.lightness();

    if( l > 160 )
    {
        mTextColor = Qt::black;
        mTitleDropShadowColor = Qt::white;
        mDropShadowShift = 1;
    }
    else
    {
        mTextColor = Qt::white;
        mTitleDropShadowColor = QColor( 50, 50, 50 );
        mDropShadowShift = -1;
    }

    mTitleDropShadowColor.setAlpha( DEFAULT_DROP_SHADOW_ALPHA );
    QString rgb = QString( "%1, %2, %3, %4" ).arg(  QString::number( mTextColor.red() ),
                                                    QString::number( mTextColor.green() ),
                                                    QString::number( mTextColor.blue() ),
                                                    "255" );
    mTitleLabel->setStyleSheet( "color: rgb(" + rgb + "); background:transparent; border: none;" );

    mCloseButton->SetColor( mTextColor );

    mTitleDropShadowEffect->setColor( mTitleDropShadowColor );
    mTitleDropShadowEffect->setOffset( 0, mDropShadowShift );

    repaint();
}



QString
Tab::GetTitle()  const
{
    return  mTitleText;
}


QString
Tab::GetTitleEllided()  const
{
    return  mTitleLabel->text();
}


Tab::eTabShape
Tab::GetTabStyle()  const
{
    return  mTabStyle;
}


const  QColor&
Tab::GetColor()  const
{
    return mBaseColor;
}


void
Tab::SetFadeColor( const  QColor& iColor )
{
    mFadeColor = iColor;
}


const  QColor&
Tab::GetFadeColor()  const
{
    return  mFadeColor;
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Tab State accessors


void
Tab::SetActive( bool iValue )
{
    mActive = iValue;
    mPressed = false;
    mOpacityEffect->setEnabled( !iValue );

    if( !mActive )
    {
        QString rgb = QString( "%1, %2, %3, %4" ).arg(  QString::number( mTextColor.red() ),
                                                    QString::number( mTextColor.green() ),
                                                    QString::number( mTextColor.blue() ),
                                                    "127" );
        mTitleLabel->setStyleSheet( "color: rgb(" + rgb + "); background:transparent; border: none;" );
    }
    else
    {
        QString rgb = QString( "%1, %2, %3, %4" ).arg(  QString::number( mTextColor.red() ),
                                                    QString::number( mTextColor.green() ),
                                                    QString::number( mTextColor.blue() ),
                                                    "255" );
        mTitleLabel->setStyleSheet( "color: rgb(" + rgb + "); background:transparent; border: none;" );
    }

    repaint();
}


bool
Tab::IsHovered()  const
{
    return  mHovered;
}


bool
Tab::IsPressed()  const
{
    return  mPressed;
}


bool
Tab::IsActive()  const
{
    return  mActive;
}


//--------------------------------------------------------------------------------------
//---------------------------------------------------------------- Behaviour constraints


void
Tab::SetLiftable( bool iValue )
{
    mLiftable = iValue;
}


void
Tab::SetClosable( bool iValue )
{
    mClosable = iValue;
    mCloseButton->setVisible( mClosable );
}


bool
Tab::IsLiftable()  const
{
    return  mLiftable;
}


bool
Tab::IsClosable()  const
{
    return  mClosable;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------- ID tag Interface


void
Tab::SetTag( const  QString&  iTag )
{
    mTag = iTag;
}


const  QString&
Tab::GetTag()  const
{
    return  mTag;
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- CB API


void
Tab::SetOnTabDroppedOutCB( OnTabDroppedOutCB iOnTabDroppedOutCB )
{
    mOnTabDroppedOutCB = iOnTabDroppedOutCB;
}


OnTabDroppedOutCB
Tab::GetOnTabDroppedOutCB()  const
{
    return  mOnTabDroppedOutCB;
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Link API


void
Tab::SetLinkWidget( QWidget* iWidget )
{
    mLinkWidget = iWidget;
}


QWidget*
Tab::GetLinkWidget()  const
{
    return  mLinkWidget;
}


////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE API                                     ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Qt Events overrides


void
Tab::resizeEvent( QResizeEvent *event )
{
    // Triggered on resize, most likely while inside a tab area, upon reorder.
    tSuperClass::resizeEvent(event);
    // Delegate compose internal geometry
    Compose();
    // Update mAnimationTargetGeometry
    mAnimationTargetGeometry = geometry();
}


void
Tab::enterEvent( QEvent* event )
{
    tSuperClass::enterEvent( event );

    mHovered = true;
    repaint();
}


void
Tab::leaveEvent( QEvent* event )
{
    tSuperClass::leaveEvent( event );

    mHovered = false;
    repaint();
}


void
Tab::mousePressEvent( QMouseEvent* event )
{
    // Mouse press triggered when inside tabArea
    // We enable only if left click
    if( event->button() == Qt::LeftButton )
    {
        // start tracking
        setMouseTracking( true );
        // Compute shift so it follows the mouse where we actually clicked
        mDragShift = event->pos();
        // Enable drag for mouse move
        mDragEnabled = true;
        // Emit message for external logic handling by docking manager
        emit  Selected( this );
        // Set to pressed for drawing
        mPressed = true;
        // Set this as active for drawing & ordering
        SetActive();
        // Trigger invalid render
        repaint();
    }
    else if( event->button() == Qt::MiddleButton )
    {
        emit  CloseClicked( this );
    }
}


void
Tab::mouseMoveEvent( QMouseEvent* event )
{
    // Moving while pressed & dragging tab when docked
    int threshold = 5;
    int dx = abs( mDragShift.x() - event->pos().x() );
    int dy = abs( mDragShift.y() - event->pos().y() );
    // Compute a distance treshold before undocking
    bool cx = dx > threshold;
    bool cy = dy > threshold;
    // Disable undocking if clicking inside a children button
    bool inButton = mCloseButton->IsPressed();

    // Check list:
    // Left button down
    // Not already dragging
    // Threshold is big enough in X or Y
    // We are not pressing in button
    // Drag enabled since last click security
    // Liftable
    if( event->buttons() == Qt::LeftButton && !mDragging && ( cx || cy ) && !inButton && mDragEnabled && mLiftable )
    {
        // Stop any animated movement
        StopAnimatedMovement();
        // Move the widget to global cursor pos before lifting to avoid flickering on lift.
        move( event->globalPos() - mDragShift );
        // Set dragging flag on
        mDragging = true;
        // Emit lifted signal, will be processed in parent tabArea & docking manager
        emit  Lifted( this );
    }
}


void
Tab::mouseReleaseEvent( QMouseEvent* event )
{
    // Trigger finish drag on release
    FinishDrag();
    mPressed = false;
    repaint();
}


void
Tab::paintEvent( QPaintEvent* event )
{
    QPainter painter(this);

    // Computing colors
    float  desaturationFactor = 0.75f;
    float  valueFactor = 0.95f;

    int ch, cs, cv, ca;
    mBaseColor.getHsl( &ch, &cs, &cv, &ca );
    QColor bgColor = QColor::fromHsl( ch, cs * desaturationFactor, cv * valueFactor, ca );
    float shadeFactor = 3.f;
    QColor outColor = QColor( bgColor.red() / shadeFactor, bgColor.green() / shadeFactor, bgColor.blue() / shadeFactor, bgColor.alpha() );

    float lightFactor = 1.2f;
    float lightConstant = 0.f;
    if( mHovered )
    {
        lightFactor = 1.5f;
        lightConstant = 40.f;
    }

    if( mPressed )
    {
        lightFactor = 0.8f;
        lightConstant = 0.f;
    }

    // Small clamp utility
    std::function<int (int, int)> fmin = [](int i, int o) { return i < o ? i : o; };

    QColor lightColor = QColor( fmin( bgColor.red()    * lightFactor + lightConstant, 255 ),
                                fmin( bgColor.green()  * lightFactor + lightConstant, 255 ),
                                fmin( bgColor.blue()   * lightFactor + lightConstant, 255 ),
                                bgColor.alpha() );

    float ultraLightFactor = 1.8f;
    float ultraLightConstant = 50.f;
    QColor ultraLightColor = QColor(    fmin( bgColor.red()    * ultraLightFactor + ultraLightConstant, 255 ),
                                        fmin( bgColor.green()  * ultraLightFactor + ultraLightConstant, 255 ),
                                        fmin( bgColor.blue()   * ultraLightFactor + ultraLightConstant, 255 ),
                                        bgColor.alpha() );

    // Computing path
    QPainterPath bezierPath;
    int h = height();
    int w = width();
    int p = mSlopePadding;
    int p2 = p / 2;
    int wp = w - p;

    if( Docked() )
    {
        switch( mTabStyle )
        {
            case  eTabShape::kLine:
            {
                painter.setRenderHint( QPainter::Antialiasing, true );
                bezierPath.moveTo( 0, h );
                bezierPath.lineTo( float( p2 ), 0.5f );
                bezierPath.lineTo( float( w - p2 ), 0.5f );
                bezierPath.lineTo( w, h );
                break;
            }

            case  eTabShape::kBezier:
            {
                painter.setRenderHint( QPainter::Antialiasing, true );
                bezierPath.moveTo( 0, h );
                bezierPath.cubicTo( p2, h, p2, 0, p, 0.5f );
                bezierPath.lineTo( wp, 0.5f );
                bezierPath.cubicTo( wp + p2, 0, w - p2, h, w, h );
                break;
            }

            case  eTabShape::kRect:
            {
                painter.setRenderHint( QPainter::Antialiasing, false );
                bezierPath.moveTo( 0, h );
                bezierPath.lineTo( 0, 0 );
                bezierPath.lineTo( w - 1, 0 );
                bezierPath.lineTo( w - 1, h );
                break;
            }

            case  eTabShape::kRect_Line:
            {
                painter.setRenderHint( QPainter::Antialiasing, false );
                bezierPath.moveTo( 0, h );
                bezierPath.lineTo( 0, 0 );
                bezierPath.lineTo( float( w - p ), 0 );
                bezierPath.lineTo( w, h );
                break;
            }
        }
    }
    else
    {
        painter.setRenderHint( QPainter::Antialiasing, true );
        int radius = 25;
        bezierPath.addRoundRect( QRect( QPoint( 0,0 ), size() ).adjusted( 1, 1, -1, -1 ), radius );
    }

    // Bg drawing with dark outline
    QLinearGradient gradient( 0, 0, 0, h );
    gradient.setColorAt(0.0, lightColor );
    gradient.setColorAt(1.0, bgColor );
    bezierPath.setFillRule( Qt::FillRule::OddEvenFill );
    //painter.setBrush( bgColor );
    painter.setBrush( gradient );
    painter.setPen( QPen( outColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin ) );
    painter.drawPath( bezierPath );

    // Light Outline Drawing
    painter.setBrush( Qt::NoBrush );
    painter.setPen( QPen( ultraLightColor, 0.5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin ) );
    painter.drawPath( bezierPath.translated( 0, 1 ) );

    if( !mActive )
    {
        QColor fadeColor = mFadeColor;
        if( mHovered )
            fadeColor.setAlpha( 30 );

        painter.setBrush( fadeColor );
        painter.setPen( QPen( fadeColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin ) );
        painter.drawPath( bezierPath );
    }
}


void
Tab::closeEvent( QCloseEvent* event )
{
    if( mLinkWidget )
        mLinkWidget->close();
}


//--------------------------------------------------------------------------------------
//----------------------------------------------------- Private GUI Processing Functions


void
Tab::Init()
{
    if( !mInvisibleBackgroundRect )     mInvisibleBackgroundRect    = new QWidget( this );
    if( !mTitleLabel )                  mTitleLabel                 = new QLabel( this );
    if( !mCloseButton )                 mCloseButton                = new RCustomButton( this );
}


void
Tab::Build()
{
    // Setup tab attributes
    setWindowFlags( Qt::FramelessWindowHint | Qt::SubWindow );
    setAttribute( Qt::WA_TranslucentBackground );
    setAttribute( Qt::WA_DeleteOnClose );

    // Setup BG rect attributes allowing for events to be propagated properly in the tab.
    mInvisibleBackgroundRect->setAttribute( Qt::WA_TranslucentBackground );

    SetTitle( mTitleText );
    CheckTitleEllipsis();

    //Temporary Debug style sheet should be moved to external qss file
    mTitleLabel->setStyleSheet( "color: rgb( 255, 255, 255 ); background:transparent; border: none;" );

    // Setting up close button properties
    mCloseButton->SetIconShape( RCustomButton::eButtonIconShape::kClose );
    mCloseButton->SetBackgroundShape( RCustomButton::eButtonBackgroundShape::kDisk );
    mCloseButton->SetHoveredBackgroundColor( QColor( 127, 127, 127, 80 ) );
    mCloseButton->SetPressedBackgroundColor( QColor( 0, 0, 0, 80 ) );
    mCloseButton->SetIconSize( 5 );

    // Drop shadow behind tab when docked
    mGlobalDropShadowEffect = new QGraphicsDropShadowEffect( this );
    mGlobalDropShadowEffect->setBlurRadius( 8 );
    mGlobalDropShadowEffect->setColor( QColor( 0, 0, 0, 255 ) );
    mGlobalDropShadowEffect->setOffset( 0, 0 );
    mGlobalDropShadowEffect->setEnabled( true );
    setGraphicsEffect( mGlobalDropShadowEffect );

    // Black border on top side of the text label
    mTitleDropShadowEffect = new QGraphicsDropShadowEffect( mTitleLabel );
    mTitleDropShadowEffect->setBlurRadius( 1 );
    mTitleDropShadowEffect->setColor( QColor( 0, 0, 0, 100 ) );
    mTitleDropShadowEffect->setOffset( 0, -1 );
    mTitleDropShadowEffect->setEnabled( true );
    mTitleLabel->setGraphicsEffect( mTitleDropShadowEffect );

    QObject::connect( mCloseButton, SIGNAL( clicked() ), this, SLOT( ProcessCloseClicked() ) );
    mOpacityEffect = new  QGraphicsOpacityEffect( this );
    mOpacityEffect->setOpacity( 0.5 );
    mCloseButton->setGraphicsEffect( mOpacityEffect );
    mCloseButton->setVisible( mClosable );
}


void
Tab::Compose()
{
    // Composition of the internal geometry

    int w = width();
    int h = height();
    int buttonPadding = h / 5;
    int buttonHeight = h - buttonPadding * 2;
    QSize btnSize( buttonHeight, buttonHeight );
    QPoint btnPosition( w - buttonHeight, buttonPadding );

    // Computing different positions according to the current tab style.
    int shift = 1;
            if( mTabStyle == eTabShape::kLine )      shift = 3;
    else    if( mTabStyle == eTabShape::kBezier )    shift = 4;

    btnPosition.setX( btnPosition.x() - buttonPadding * shift );

    // mInvisibleBackgroundRect should always be as large as the tab
    mInvisibleBackgroundRect->setGeometry( QRect( QPoint( 0,0 ), size() ) );

    mCloseButton->setGeometry( QRect( btnPosition, btnSize ) );
    int computedPaddingRight = w - mCloseButton->geometry().right();
    mTitleLabel->setGeometry( computedPaddingRight, buttonPadding, mCloseButton->geometry().x() - computedPaddingRight, buttonHeight );

    CheckTitleEllipsis();
}


void
Tab::Destroy()
{
    delete  mTitleDropShadowEffect;
    delete  mGlobalDropShadowEffect;
    delete  mOpacityEffect;
    delete  mTitleLabel;
    delete  mCloseButton;
    delete  mInvisibleBackgroundRect;

    mTitleLabel                 = 0;
    mCloseButton                = 0;
    mInvisibleBackgroundRect    = 0;
    mTitleDropShadowEffect      = 0;
    mGlobalDropShadowEffect     = 0;
    mOpacityEffect              = 0;
}


//--------------------------------------------------------------------------------------
//---------------------------------------------------- Internal Tab Utilities on Compose


void
Tab::CheckTitleEllipsis()
{
    // Reset title to saved text & recheck for ellipsis
    SetTitle( mTitleText );
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------ Docking Manager Registering API


void
Tab::Register()
{
    DockingManager()->RegisterTab( this );
}


void
Tab::Unregister()
{
    DockingManager()->UnregisterTab( this );
}


////////////////////////////////////////////////////////////////////////////////////////
////                              SIGNAL SLOTS API                                  ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Tab Slots


void
Tab::ProcessCloseClicked()
{
    if( Dragging() || !mClosable )
        return;

    if( Docked() )
        emit  CloseClicked( this );
    else
        tSuperClass::close();
}


} // namespace  Rivet

