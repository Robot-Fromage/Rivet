/**
*
*   Rivet
*__________________
*
* @file     Rivet.__private__.AbstractFramelessBlurBehindWindow.cpp
* @author   Clement Berthaud
* @brief    This file provides the definition for the IFramelessTranslucentBlurBehindWindow class.
*/
#include "Rivet/__private__/Rivet.__private__.AbstractFramelessBlurBehindWindow.h"
#include "Rivet/__private__/Rivet.__private__.WinExtras.h"
#include <QtWinExtras>

namespace  Rivet {
namespace  __private__ {
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default values defines
#define  DEFAULT_BORDER_WIDTH 8

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Utilities defines
#define EXTRACT_X( lp )  ( ( int )( short ) LOWORD( lp ) )
#define EXTRACT_Y( lp )  ( ( int )( short ) HIWORD( lp ) )

//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
//virtual
IFramelessTranslucentBlurBehindWindow::~IFramelessTranslucentBlurBehindWindow()
{
}


IFramelessTranslucentBlurBehindWindow::IFramelessTranslucentBlurBehindWindow( QWidget* iParent )
    : tSuperClass(  iParent                 )
    , mBorderWidth( DEFAULT_BORDER_WIDTH    )
{
    // The functions are called in this order, and this is important.
    // otherwise some flags are overriden in an unwanted way and init isn't done properly.
    setAttribute( Qt::WA_TranslucentBackground, true );
    InitNativeFrameless();
}


//--------------------------------------------------------------------------------------
//----------------------------------------------------- Protected Non-Client OS geometry
int
IFramelessTranslucentBlurBehindWindow::BorderWidth()  const
{
    return  mBorderWidth;
}


int
IFramelessTranslucentBlurBehindWindow::DefaultBorderWidth()  const
{
    return  DEFAULT_BORDER_WIDTH;
}


void
IFramelessTranslucentBlurBehindWindow::SetBorderWidth( int iValue )
{
    mBorderWidth = iValue;
}


void
IFramelessTranslucentBlurBehindWindow::ResetBorderWidth()
{
    SetBorderWidth( DefaultBorderWidth() );
}


//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- Public Blur API
void
IFramelessTranslucentBlurBehindWindow::EnableBlurBehind()
{
    // We enable blur in different ways according to windows version
    // That way all windows versions are covered since windows vista, simple enough.
    ::Rivet::__private__::eWindowsVersion v = ::Rivet::__private__::GetWindowsVersion();

    // The simple way with DWM access wrapped by Qt WinExtras for windows vista & 7
    if( v >= ::Rivet::__private__::eWindowsVersion::kWindowsVista && v < ::Rivet::__private__::eWindowsVersion::kWindows8 )
        QtWin::enableBlurBehindWindow( this );

    // The hard hacky way with to undocumented windows API, for windows 8 & 10
    if( v >= ::Rivet::__private__::eWindowsVersion::kWindows8 )
        ::Rivet::__private__::EnableGlassForWindow8OrGreater( (HWND)winId() );
}


void
IFramelessTranslucentBlurBehindWindow::DisableBlurBehind()
{
    // Do not set autofill background because it messes up the background erasing,
    // and transparent shapes will be painted over & not erased
    //setAutoFillBackground( true );

    // This flag is important for seeing behing the Qt painting of widgets and actually seeing the blur behind.
    setAttribute( Qt::WA_TranslucentBackground, false );

    // We enable blur in different ways according to windows version
    // That way all windows versions are covered since windows vista, simple enough.
    ::Rivet::__private__::eWindowsVersion v = ::Rivet::__private__::GetWindowsVersion();

    // The simple way with DWM access wrapped by Qt WinExtras for windows vista & 7
    if( v >= ::Rivet::__private__::eWindowsVersion::kWindowsVista && v < ::Rivet::__private__::eWindowsVersion::kWindows8 )
        QtWin::disableBlurBehindWindow( this );

    // The hard hacky way with to undocumented windows API, for windows 8 & 10
    if( v >= ::Rivet::__private__::eWindowsVersion::kWindows8 )
        ::Rivet::__private__::DisableGlassForWindow8OrGreater( (HWND)winId() );
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------ Private Win32 flags setup
void
IFramelessTranslucentBlurBehindWindow::InitNativeFrameless()
{
    // Drop shadow, this is redundant but not conflicting with windows 8 ( or greater ) way of setting glass.
    QtWin::extendFrameIntoClientArea( this, 1, 1, 1, 1 );

    // Frameless Style, yet with some Custom Flags that enable aero snap behaviour.
    auto framelessStyle = ( WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX );
    ::SetWindowLongPtrW( (HWND)winId(), GWL_STYLE, framelessStyle );
}


//--------------------------------------------------------------------------------------
//------------------------------------------------- Private Non-Client OS event handling
void
IFramelessTranslucentBlurBehindWindow::WM_NCHITTEST_Event_Handler( int iX, int iY, long* oResult )
{
    // Custom Handling of NonClient Event, normally handled by the native Caption.
    // Here we handle native OS resize & move events.
    // This allows us to have custom frame style, yet handle correctly some native
    // events, like window snapping on borders.

    // Security, ensure *oResult = 0 as this will be the return result in case
    // nothing is handled in this function, that means we are in client area and
    // the Non Client Event shouldn't be processed.
    assert( oResult );
    *oResult = 0;

    // Collect rect and size data
    bool resizeHorizontalEnabled    = minimumWidth() != maximumWidth();
    bool resizeVerticalEnabled      = minimumHeight() != maximumHeight();

    // cast the data to be compatible with windows API stuff.
    const LONG border_width = mBorderWidth;
    RECT winrect;
    GetWindowRect( reinterpret_cast< HWND >( winId() ), &winrect );
    QRect rect( winrect.left, winrect.top, winrect.right - winrect.left, winrect.bottom - winrect.top );

    // Handle resize horizontal on left & right borders
    if( resizeHorizontalEnabled )
    {
        if( NCHitLeftBorder(    rect, border_width, iX, iY) )   *oResult = HTLEFT;
        if( NCHitRightBorder(   rect, border_width, iX, iY) )   *oResult = HTRIGHT;
    }

    // Handle resize vertical on top & bot borders
    if( resizeVerticalEnabled )
    {
        if( NCHitBottomBorder(   rect, border_width, iX, iY) )  *oResult = HTBOTTOM;
        if( NCHitTopBorder(      rect, border_width, iX, iY) )  *oResult = HTTOP;
    }

    // Handle resize on corners
    if( resizeHorizontalEnabled && resizeVerticalEnabled )
    {
        if( NCHitBotLeftCorner(  rect, border_width, iX, iY) )  *oResult = HTBOTTOMLEFT;
        if( NCHitBotRightCorner( rect, border_width, iX, iY) )  *oResult = HTBOTTOMRIGHT;
        if( NCHitTopLeftCorner(  rect, border_width, iX, iY) )  *oResult = HTTOPLEFT;
        if( NCHitTopRightCorner( rect, border_width, iX, iY) )  *oResult = HTTOPRIGHT;
    }

    // Handle drag move based on hit caption test
    // Note NCHitCaption is generally overriden in children of this class.
    if(*oResult==0)
    {
        if( NCHitCaption( rect, border_width, iX, iY ) )        *oResult = HTCAPTION ;
    }
}


bool
IFramelessTranslucentBlurBehindWindow::NCHitLeftBorder( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    return  iX >= iRect.left() && iX < iRect.left() + iBorderWidth;
}


bool
IFramelessTranslucentBlurBehindWindow::NCHitRightBorder( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    return  iX < iRect.right() && iX >= iRect.right() - iBorderWidth;
}


bool
IFramelessTranslucentBlurBehindWindow::NCHitTopBorder( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    return  iY >= iRect.top() && iY < iRect.top() + iBorderWidth;
}


bool
IFramelessTranslucentBlurBehindWindow::NCHitBottomBorder( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    return  iY < iRect.bottom()&& iY >= iRect.bottom() - iBorderWidth;
}


bool
IFramelessTranslucentBlurBehindWindow::NCHitTopLeftCorner( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    return  NCHitTopBorder( iRect, iBorderWidth, iX, iY ) &&
            NCHitLeftBorder( iRect, iBorderWidth, iX, iY );
}


bool
IFramelessTranslucentBlurBehindWindow::NCHitTopRightCorner( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    return  NCHitTopBorder( iRect, iBorderWidth, iX, iY ) &&
            NCHitRightBorder( iRect, iBorderWidth, iX, iY );
}


bool
IFramelessTranslucentBlurBehindWindow::NCHitBotRightCorner( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    return  NCHitBottomBorder( iRect, iBorderWidth, iX, iY ) &&
            NCHitRightBorder( iRect, iBorderWidth, iX, iY );
}


bool
IFramelessTranslucentBlurBehindWindow::NCHitBotLeftCorner( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    return  NCHitBottomBorder( iRect, iBorderWidth, iX, iY ) &&
            NCHitLeftBorder( iRect, iBorderWidth, iX, iY );
}


//--------------------------------------------------------------------------------------
//----------------------------------------------- Protected Non-Client OS event handling
bool
IFramelessTranslucentBlurBehindWindow::NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    // Default implementation can be overriden in childs of this class.
    return  true;
}


//--------------------------------------------------------------------------------------
//------------------------------------------ Protected Qt / Win32 native events override
bool
IFramelessTranslucentBlurBehindWindow::nativeEvent( const  QByteArray&  eventType, void* message, long* result)
{
    // Cast to Win32 API types.
    MSG*    wmsg    = reinterpret_cast< MSG* >( message );
    LPARAM  lparam  = wmsg->lParam;
    WPARAM  wparam  = wmsg->wParam;
    HWND    hwnd    = wmsg->hwnd;
    UINT    msg     = wmsg->message;

    switch( msg )
    {
        // Native geometry computing and drawing of the non client area.
        case WM_NCCALCSIZE:
        {
            // Abort computing & drawing of the non client area by setting result to 0.
            // Return true ( do not propagate any further )
            // Event is handled with result 0 is important.
            *result = 0;
            return  true;
        }

        // Handle non-client area snap resize & drag behaviours & detection
        case WM_NCHITTEST:
        {
            int x = EXTRACT_X( lparam );
            int y = EXTRACT_Y( lparam );

            // Custom handling delegate
            WM_NCHITTEST_Event_Handler( x, y, result );

            // If any test succeeded, return true, stop propagation
            // See WM_NCHITTEST_Event_Handler for details
            if( *result != 0 )
                return  true;
            else
                return  tSuperClass::nativeEvent( eventType, message, result );
        }

        // The case is not handled ? Treat it the normal way & return.
        default:
        {
            return  tSuperClass::nativeEvent( eventType, message, result );
        }

    } // switch( msg )
}


} // namespace  __private__
} // namespace  Rivet

