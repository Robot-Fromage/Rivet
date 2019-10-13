/**
*
*   Rivet
*__________________
*
* @file     Rivet.__private__.CustomMainWindowBase.cpp
* @author   Clement Berthaud
* @brief    This file provides the definition for the RCustomMainWindowBase class.
*/
#include "Rivet/__private__/Rivet.__private__.CustomMainWindowBase.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <Windows.h> // For GetSystemMetrics

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default values defines
#define  DEFAULT_CAPTION_HEIGHT 30
#define  DEFAULT_PADDING_LEFT   0
#define  DEFAULT_PADDING_TOP    0
#define  DEFAULT_PADDING_RIGHT  0
#define  DEFAULT_PADDING_BOTTOM 0
#define  DEFAULT_PADDINGS       DEFAULT_PADDING_LEFT, DEFAULT_PADDING_TOP, DEFAULT_PADDING_RIGHT, DEFAULT_PADDING_BOTTOM

namespace  Rivet {
namespace  __private__ {
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
RCustomMainWindowBase::~RCustomMainWindowBase()
{
}


RCustomMainWindowBase::RCustomMainWindowBase( QWidget* iParent )
    : tSuperClass(              iParent                 )
    , mCaptionHeight(           DEFAULT_CAPTION_HEIGHT  )
    , mPaddings(                DEFAULT_PADDINGS        )
    , mRestoreGeometry(         QRect()                 )
    , mMaximized(               false                   )
    , mLatestOldPos(            QPoint()                )
    , mLatestOldSize(           QSize()                 )
    , mIsResizing(              false                   )
    , mOverrideContentsMargins( QMargins()              )
{
    // Initialize mLatestOldPos in order to avoid it being restored in invalid location when restored if never moved before.
    mLatestOldPos = QApplication::desktop()->availableGeometry( this ).topLeft();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Internal Client Geometry API
void
RCustomMainWindowBase::SetCaptionHeight( int iValue )
{
    mCaptionHeight = iValue;
}


int
RCustomMainWindowBase::GetCaptionHeight()  const
{
    return  mCaptionHeight;
}


QRect
RCustomMainWindowBase::CaptionGeometry()  const
{
    // Get Geometyry & Height of the title caption part, starting from top.
    int x = mOverrideContentsMargins.left();
    int y = mOverrideContentsMargins.top();
    return  QRect( x, y, width() - 2 * x, mCaptionHeight );
}


QRect
RCustomMainWindowBase::ContentsGeometry()  const
{
    // Get Geometry of the contents part.
    int x = mOverrideContentsMargins.left();
    int y = mOverrideContentsMargins.top();
    return  QRect(  x, mCaptionHeight + y, width() - 2 * x, height() - mCaptionHeight - 2 * y );
}


//--------------------------------------------------------------------------------------
//----------------------------------------------- Custom Sizing behaviour implementation
void
RCustomMainWindowBase::Restore()
{
    // Manual restore implementation
    // First, the Regular way.
    showNormal();

    // This is not really necessary:
    //setGeometry( mRestoreGeometry );

    // The Patch geometry handles cases where snap & maximize conflict.
    AdjustWindowOnRestore();

    // The flag reset for consistency.
    mMaximized = false;
    mIsResizing = false;
}


bool
RCustomMainWindowBase::CheckCustomWindowMaximizedState()  const
{
    return  mMaximized;
}


bool
RCustomMainWindowBase::CheckCustomWindowResizingState()  const
{
    return  mIsResizing;
}


//--------------------------------------------------------------------------------------
//----------------------------------------------- Protected Non-Client OS event handling
bool
RCustomMainWindowBase::NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    // This Transition implementation can be called or overriden in childs of this class.
    // This one allows dragging only if inside caption geometry.

    // Custom processing for sending "hit caption" message to the OS
    // Return true:  the OS handles window move under mouse cursor like a native window
    // Return false: the mouse Events remain within the window
    int y = mOverrideContentsMargins.top();
    bool    elligible = iY - y >= iRect.top() && iY - y < iRect.top() + GetCaptionHeight();
    return  elligible;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt events override
void
RCustomMainWindowBase::resizeEvent( QResizeEvent* event )
{
    // Patch window behaviour and size on Maximize according to the available geometry
    mIsResizing = true;

    // This is for the custom Maximize behaviour.
    QRect availableRect = QApplication::desktop()->availableGeometry( this );
    if( event->oldSize().width() < availableRect.width() - 1 && event->oldSize().height() < availableRect.height() - 1 )
        mLatestOldSize = event->oldSize();

    // On Restore Behaviour Patch ( here applied for win7 )
    AdjustWindowOnRestore();

    // Avoid having the window stuck behind taskbar
    FixWindowOverlapOnResize( event->oldSize() );

    // Fix for move too since resizing on some sides is equivalent to a move.
    FixWindowOverlapOnMove();
}


void
RCustomMainWindowBase::moveEvent( QMoveEvent* event )
{
    // Patch window behaviour and size on Maximize according to the available geometry
    QRect availableRect = QApplication::desktop()->availableGeometry( this );
    if( event->oldPos().x() > availableRect.x() + 1 && event->oldPos().y() > availableRect.y() + 1 )
        mLatestOldPos = event->oldPos();

    // Avoid having the window stuck behind taskbar
    FixWindowOverlapOnMove();
}


void
RCustomMainWindowBase::changeEvent(QEvent* event)
{
    // Regular processing anyways
    tSuperClass::changeEvent(event);

    // + Additional Behaviour Patch Processing
    if( !( event->type() == QEvent::WindowStateChange ) )
        return;

    // Handling special behaviour for maximized state on windows state change
    if( isMaximized() )
    {
        // On Maximized Behaviour Patch
        AdjustWindowOnMaximize();

        // + set restore geometry manually for later.
        mRestoreGeometry = QRect( mLatestOldPos, mLatestOldSize );
    }
    else
    {
        // On Restore Behaviour Patch ( win10 )
        AdjustWindowOnRestore();
    }
}


//--------------------------------------------------------------------------------------
//---------------------------------------------------- Private Window behaviours patches
void
RCustomMainWindowBase::FixWindowOverlapOnMove()
{
    // Collect screen & geometry information on move
    QRect availableRect = QApplication::desktop()->availableGeometry( this );
    QRect thisRect      = geometry().adjusted( 0, mCaptionHeight, 0, 0 );
    int shift = mCaptionHeight;
    // If the Rect of the window is outside of the bounds ( taking the caption height into account )
    if( !thisRect.intersects( availableRect ) )
    {
        // We move the window in bounds
        if( thisRect.bottom() < availableRect.top() )   move( thisRect.x(), availableRect.top()    + shift );
        if( thisRect.top() > availableRect.bottom() )   move( thisRect.x(), availableRect.bottom() - shift );
        if( thisRect.right() < availableRect.left() )   move( availableRect.left()    + shift,  thisRect.y() );
        if( thisRect.left() > availableRect.right() )   move( availableRect.right()   - shift,  thisRect.y() );
    }
}


void
RCustomMainWindowBase::FixWindowOverlapOnResize( const  QSize&  iOldSize )
{
    // Collect screen & geometry information on resize 
    QRect availableRect = QApplication::desktop()->availableGeometry( this );
    QRect thisRect      = geometry().adjusted( 0, mCaptionHeight, 0, 0 );

    // If the Rect of the window is outside of the bounds ( taking the caption height into account )
    if( !thisRect.intersects( availableRect ) )
    {
        // We forbid resizing
        resize( iOldSize );
    }
}


void
RCustomMainWindowBase::AdjustWindowOnMaximize()
{
    if( !isMaximized() )
        return;

    // SM_CXFRAME SM_CYFRAME, The codes for GetSystemMetrics, border metrics
    int quarterPounderWithCheese = 92; // SM_CXPADDEDBORDER
    const int x = GetSystemMetrics( SM_CXFRAME ) + GetSystemMetrics( quarterPounderWithCheese );
    const int y = GetSystemMetrics( SM_CYFRAME ) + GetSystemMetrics( quarterPounderWithCheese );
    mOverrideContentsMargins = QMargins( x, y, x, y );

    // Disable OS resize on border when maximized
    tSuperClass::SetBorderWidth( 0 );

    // Set manual states
    mMaximized  = true;
    mIsResizing = false;

    setContentsMargins({x, y, x, y});
}


void
RCustomMainWindowBase::AdjustWindowOnRestore()
{
    // When check for maximized from Qt method
    if( isMaximized() )
        return;

    // Reset the override content margins storage
    mOverrideContentsMargins = QMargins( 0, 0, 0, 0 );

    // Reset the OS border width.
    ResetBorderWidth();

    // Remove the manual maximized flag ( can be redundant but safe )
    mMaximized = false;

    // Reset the content margins from Qt method
    setContentsMargins( { 0, 0, 0, 0 } );
}


} // namespace  __private__
} // namespace  Rivet

