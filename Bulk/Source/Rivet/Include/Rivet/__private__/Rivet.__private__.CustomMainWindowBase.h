/**
*
*   Rivet
*__________________
*
* @file     Rivet.__private__.CustomMainWindowBase.h
* @author   Clement Berthaud
* @brief    This file provides the declaration for the RCustomMainWindowBase class.
*/
#pragma once
#include "Rivet.__private__.AbstractFramelessBlurBehindWindow.h"

namespace  Rivet {
namespace  __private__ {
/////////////////////////////////////////////////////
/// @class      RCustomMainWindowBase
/// @brief      The RCustomMainWindowBase class provides a base framework for Translucent windows with BlurBehind.
/// @details    Secondary level of abstraction for custom windows.
///             Handles Qt events and Qt API calls.
///             Users should not derive this class.
///             See usage in \e RCustomMainWindow.
///             It is made part of the public header distribution for inheritance constraint.
///             It inherits IFramelessTranslucentBlurBehindWindow.
class RCustomMainWindowBase
    : public IFramelessTranslucentBlurBehindWindow
{
private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Typedefs
    typedef  RCustomMainWindowBase                      tSelf;
    typedef  IFramelessTranslucentBlurBehindWindow      tSuperClass;


public:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
    /// @fn         virtual  ~RCustomMainWindowBase()
    /// @brief      Default Destructor.
    /// @details    virtual, does nothing.
    virtual  ~RCustomMainWindowBase();


    /// @fn         RCustomMainWindowBase( QWidget* iParent = nullptr )
    /// @brief      Default Constructor.
    /// @details    Init member data.
    RCustomMainWindowBase( QWidget* iParent = nullptr );


public:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Internal Client Geometry API
    /// @fn         void  SetCaptionHeight( int iValue )
    /// @brief      Setter for the Caption Height.
    /// @details    Set the caption height to the desired value.
    /// @param      iValue      The desired caption height.
    void    SetCaptionHeight( int iValue );


    /// @fn         int  GetCaptionHeight()
    /// @brief      Getter for the Caption Height.
    /// @details    Get the current caption height.
    /// @return     An integer value representing the caption height.
    int     GetCaptionHeight()  const;


    /// @fn         QRect  CaptionGeometry()
    /// @brief      Getter for the Caption Geometry.
    /// @details    The rect is in client space and takes paddings & margins into account.
    /// @return     A QRect representing the Caption Geometry.
    QRect   CaptionGeometry()  const;


    /// @fn         QRect  ContentsGeometry()
    /// @brief      Getter for the Contents Geometry.
    /// @details    The rect is in client space and takes paddings & margins into account.
    /// @return     A QRect representing the Contents Geometry.
    QRect   ContentsGeometry()  const;


public:
//--------------------------------------------------------------------------------------
//----------------------------------------------- Custom Sizing behaviour implementation
    /// @fn         void  Restore()
    /// @brief      Restore the window geometry to a non-maximized state.
    /// @details    The window will have the position and size before it was maximized in the first place.
    void  Restore();


    /// @fn         bool  CheckCustomWindowMaximizedState()  const
    /// @brief      Check if the window is maximized.
    /// @details    Using custom mechanisms over Qt methods.
    /// @return     The maximized state as a boolean value; true: maximized, false: normal.
    bool  CheckCustomWindowMaximizedState()  const;


    /// @fn         bool  CheckCustomWindowResizingState()  const
    /// @brief      Check if the window is currently being resized by OS native events.
    /// @details    Using custom mechanisms over Qt methods.
    /// @return     The resizing state as a boolean value; true: resizing, false: not-resizing.
    bool  CheckCustomWindowResizingState()  const;


protected:
//--------------------------------------------------------------------------------------
//----------------------------------------------- Protected Non-Client OS event handling
    /// @fn         virtual  bool  NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )  override
    /// @brief      Override of the Hit-Test to check if the cursor hits the caption.
    /// @details    A return value of true means the hit-test succeeded and the window can be moved and snaped natively.
    ///             A return value of false means the hit-test failed and the window will not respond to move / resize events.
    ///             You can use this to avoid moving the window if hitting a widget in the caption bar, like a button.
    /// @return     A boolean value stating the behaviour to take on caption events.
    virtual  bool  NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )  override;


protected:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt events override
    virtual  void  resizeEvent( QResizeEvent*   event )  override;
    virtual  void  moveEvent(   QMoveEvent*     event )  override;
    virtual  void  changeEvent( QEvent*         event )  override;


private:
//--------------------------------------------------------------------------------------
//---------------------------------------------------- Private Window behaviours patches
    /// @fn         void  FixWindowOverlapOnMove()
    /// @brief      Patch: fix the window overlaping the taskbar after a move.
    /// @details    If the window ends up outside of the desktop area or under the taskbar, it will be moved back into a safe area
    void  FixWindowOverlapOnMove();


    /// @fn         void  FixWindowOverlapOnResize()
    /// @brief      Patch: fix the window overlaping the taskbar after a resize.
    /// @details    If the window ends up outside of the desktop area or under the taskbar, it will be resized or moved back into a safe area
    void  FixWindowOverlapOnResize( const  QSize&  iOldSize );


    /// @fn         void  AdjustWindowOnMaximize()
    /// @brief      Patch: fix the window paddings on maximize, and sets internal state flags.
    void  AdjustWindowOnMaximize();


    /// @fn         void  AdjustWindowOnRestore()
    /// @brief      Patch: fix the window paddings on restore, and sets internal state flags.
    void  AdjustWindowOnRestore();


private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------- Private Data Members
    int         mCaptionHeight;             ///< The caption Height.
    QMargins    mPaddings;                  ///< The internal content paddings
    QRect       mRestoreGeometry;           ///< The backup restore geometry
    bool        mMaximized;                 ///< The maximized state stored as a boolean flag
    QPoint      mLatestOldPos;              ///< The latest safe position
    QSize       mLatestOldSize;             ///< The latest safe sizee
    bool        mIsResizing;                ///< The resizing state stored as a boolean flag
    QMargins    mOverrideContentsMargins;   ///< The contents margins used to control the OS borders

};

} // namespace  __private__
} // namespace  Rivet

