/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.__private__.CustomMainWindowBase.h
* 9-9-2018 13:27 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma once


#include "Rivet.__private__.AbstractFramelessBlurBehindWindow.h"


// Secondary level for custom windows
// Handles Qt events and Qt API calls
// You should not derive this class


namespace  Rivet
{
namespace  __private__
{


class cCustomMainWindowBase :
    public ::Rivet::__private__::cAbstractFramelessBlurBehindWindow
{

    typedef  ::Rivet::__private__::cAbstractFramelessBlurBehindWindow  tSuperClass;

public:
    // Construction / Destruction
    virtual  ~cCustomMainWindowBase();
    cCustomMainWindowBase( QWidget* parent = NULL );

public:
    // Internal Client Geometry Handling
    void    SetCaptionHeight( int iValue );
    int     GetCaptionHeight();
    QRect   CaptionGeometry();
    QRect   ContentsGeometry();

public:
    // Custom Maximize behaviour implementation
    void  Restore();
    bool  CheckCustomWindowMaximizedState()  const;
    bool  CheckCustomWindowResizingState()  const;

protected:
    // Protected Non-Client OS behaviour handling overrides
    virtual  bool  NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )  override;

protected:
    // Protected Qt events override
    virtual  void  resizeEvent( QResizeEvent*   event )     override;
    virtual  void  moveEvent(   QMoveEvent*     event )     override;
    virtual  void  changeEvent( QEvent*         event )     override;

private:
    // Window behaviour patch
    void  FixWindowOverlapOnMove();
    void  FixWindowOverlapOnResize( const  QSize&  iOldSize );
    void  AdjustWindowOnMaximize();
    void  AdjustWindowOnRestore();

private:
    // Private member data: internal geometry
    int         mCaptionHeight;
    QMargins    mPaddings;

    // Private member data: Custom maximized / restore behaviour related data
    QRect       mRestoreGeometry;
    bool        mMaximized;
    QPoint      mLatestOldPos;
    QSize       mLatestOldSize;
    bool        mIsResizing;
    QMargins    mOverrideContentsMargins;
};


} // namespace  __private__
} // namespace  Rivet

