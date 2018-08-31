/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.__private__.CustomMainWindowBase.cpp
*
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include "Rivet/__private__/Rivet.__private__.CustomMainWindowBase.h"


#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <Windows.h>


// Default values defines
#define  DEFAULT_CAPTION_HEIGHT 30
#define  DEFAULT_PADDING_LEFT   0
#define  DEFAULT_PADDING_TOP    0
#define  DEFAULT_PADDING_RIGHT  0
#define  DEFAULT_PADDING_BOTTOM 0
#define  DEFAULT_PADDINGS       DEFAULT_PADDING_LEFT, DEFAULT_PADDING_TOP, DEFAULT_PADDING_RIGHT, DEFAULT_PADDING_BOTTOM


namespace  Rivet
{
namespace  __private__
{


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


cCustomMainWindowBase::~cCustomMainWindowBase()
{
}


cCustomMainWindowBase::cCustomMainWindowBase( QWidget *parent ) :
    tSuperClass( parent ),
    mCaptionHeight(     DEFAULT_CAPTION_HEIGHT  ),
    mPaddings(          DEFAULT_PADDINGS        ),
    mRestoreGeometry(   QRect()                 ),
    mMaximized(         false                   ),
    mLatestOldPos(      QPoint()                ),
    mLatestOldSize(     QSize()                 ),
    mOverrideContentsMargins( QMargins()        )
{
    // Initialize mLatestOldPos in order to avoid it being restored in invalid location when restored if never moved before.
    mLatestOldPos = QApplication::desktop()->availableGeometry( this ).topLeft();
}


//--------------------------------------------------------------------------------------
//---------------------------------------------------- Internal Client Geometry Handling


void
cCustomMainWindowBase::SetCaptionHeight( int iValue )
{
    mCaptionHeight = iValue;
}


int
cCustomMainWindowBase::GetCaptionHeight()
{
    return  mCaptionHeight;
}


QRect
cCustomMainWindowBase::CaptionGeometry()
{
    // Get Geometyry & Height of the title caption part, starting from top.
    int x = mOverrideContentsMargins.left();
    int y = mOverrideContentsMargins.top();
    return  QRect( x, y, width() - 2 * x, mCaptionHeight );
}


QRect
cCustomMainWindowBase::ContentsGeometry()
{
    // Get Geometry of the contents part.
    int x = mOverrideContentsMargins.left();
    int y = mOverrideContentsMargins.top();
    return  QRect(  x, mCaptionHeight + y, width() - 2 * x, height() - mCaptionHeight - 2 * y );
}


//--------------------------------------------------------------------------------------
//--------------------------------------------- Custom Maximize behaviour implementation


void
cCustomMainWindowBase::Restore()
{
    // Manual restore implementation

    // The Regular way.
    showNormal();

    // The Patch geometry handles cases where snap & maximize conflict.
    // Is it really necessary ?
    //setGeometry( mRestoreGeometry );

    // The patch Adjust which resets the flag & borders.
    AdjustWindowOnRestore();

    // The flag reset for consistency.
    mMaximized = false;
    mIsResizing = false;
}


bool
cCustomMainWindowBase::CheckCustomWindowMaximizedState()  const
{
    // Manual maximized state implementation check:
    return  mMaximized;
}


bool
cCustomMainWindowBase::CheckCustomWindowResizingState()  const
{
    return  mIsResizing;
}


//--------------------------------------------------------------------------------------
//--------------------------------- Protected Non-Client OS behaviour handling overrides


bool
cCustomMainWindowBase::NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    // Transition implementation can be called or overriden in childs of this class.
    // This one allows dragging only if caption geometry.

    // Custom processing for sending "hit caption" message to the OS
    // Return true:  the OS handles window move under mouse cursor like a native window
    // Return false: the mouse Events remain within the window
    int y = mOverrideContentsMargins.top();
    bool  eligible = iY - y >= iRect.top() && iY - y < iRect.top() + GetCaptionHeight();

    if(!eligible)
        return  false;

    return  true;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt events override


void
cCustomMainWindowBase::resizeEvent( QResizeEvent* event )
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
cCustomMainWindowBase::moveEvent( QMoveEvent* event )
{
    // Patch window behaviour and size on Maximize according to the available geometry
    QRect availableRect = QApplication::desktop()->availableGeometry( this );
    if( event->oldPos().x() > availableRect.x() + 1 && event->oldPos().y() > availableRect.y() + 1 )
        mLatestOldPos = event->oldPos();

    // Avoid having the window stuck behind taskbar
    FixWindowOverlapOnMove();
}


void
cCustomMainWindowBase::changeEvent(QEvent* event)
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
//--------------------------------------------------------------- Window behaviour patch


void
cCustomMainWindowBase::FixWindowOverlapOnMove()
{
    // Collect screen & geometry information on move
    QRect availableRect = QApplication::desktop()->availableGeometry( this );
    QRect thisRect      = geometry().adjusted( 0, mCaptionHeight, 0, 0 );
    int shift = mCaptionHeight;
    // If the Rect of the window is outside of the bounds ( taking the caption height into account )
    if( !thisRect.intersects( availableRect ) )
    {
        // We move the window in bounds
        if( thisRect.bottom() < availableRect.top() )   move( thisRect.x(),                     availableRect.top()    + shift );
        if( thisRect.top() > availableRect.bottom() )   move( thisRect.x(),                     availableRect.bottom() - shift );
        if( thisRect.right() < availableRect.left() )   move( availableRect.left()    + shift,  thisRect.y() );
        if( thisRect.left() > availableRect.right() )   move( availableRect.right()   - shift,  thisRect.y() );
    }
}


void
cCustomMainWindowBase::FixWindowOverlapOnResize( const  QSize&  iOldSize )
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
cCustomMainWindowBase::AdjustWindowOnMaximize()
{
    if(!isMaximized())
        return;

    // Hack to go around MinGW _WIN32_WINNT
    int SM_CXPADDEDBORDER_code = 92;
    const int x = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER_code);
    const int y = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER_code);
    mOverrideContentsMargins = QMargins( x, y, x, y );

    // Disable OS resize on border when maximized
    tSuperClass::SetBorderWidth( 0 );

    // Set manual maximized state as the resizing done before disabled the os maximized flag.
    mMaximized = true;
    mIsResizing = false;

    setContentsMargins({x, y, x, y});
}


void
cCustomMainWindowBase::AdjustWindowOnRestore()
{
    // When not maximized, we make sure the border width
    // telling the OS where to resize are reset to default
    if( isMaximized() )
        return;

    mOverrideContentsMargins = QMargins( 0, 0, 0, 0 );

    // Reset the OS resize on border
    ResetBorderWidth();

    // Remove the manual maximized flag
    mMaximized = false;

    setContentsMargins( { 0, 0, 0, 0 } );
}


} // namespace  __private__
} // namespace  Rivet

