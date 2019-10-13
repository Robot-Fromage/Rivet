/**
*
*   Rivet
*__________________
*
* @file     Rivet.__private__.AbstractFramelessBlurBehindWindow.h
* @author   Clement Berthaud
* @brief    This file provides the declaration for the IFramelessTranslucentBlurBehindWindow class.
*/
#pragma once
#include <QWidget>

namespace  Rivet {
namespace  __private__ {
/////////////////////////////////////////////////////
/// @class      IFramelessTranslucentBlurBehindWindow
/// @brief      The IFramelessTranslucentBlurBehindWindow abstract class provides a base framework for Translucent windows with BlurBehind.
/// @details    Primary abstract level for custom windows.
///             Handles OS events and OS API calls.
///             Users should not derive this class.
///             See usage in \e RCustomMainWindowBase.
///             It is made part of the public header distribution for inheritance constraint.
///             It inherits QWidget.
class IFramelessTranslucentBlurBehindWindow
    : public QWidget
{
private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Typedefs
    typedef  IFramelessTranslucentBlurBehindWindow      tSelf;
    typedef  QWidget                                    tSuperClass;


public:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
    /// @fn         virtual  ~IFramelessTranslucentBlurBehindWindow() = 0
    /// @brief      Default Destructor.
    /// @details    Pure virtual, does nothing.
    virtual  ~IFramelessTranslucentBlurBehindWindow() = 0;


    /// @fn         IFramelessTranslucentBlurBehindWindow( QWidget*  parent = nullptr )
    /// @brief      Default Constructor.
    /// @details    Set default attributes and flags and inits system calls with \e InitNativeFrameless.
    ///             BlurBehind is disabled by default.
    IFramelessTranslucentBlurBehindWindow( QWidget* iParent = nullptr );


protected:
//--------------------------------------------------------------------------------------
//----------------------------------------------------- Protected Non-Client OS geometry
    /// @fn         int  BorderWidth()
    /// @brief      Getter for Border Width.
    /// @details    Get the border width used in the non-client hit-tests.
    /// @return     The current border width value.
    int     BorderWidth()  const;


    /// @fn         int  DefaultBorderWidth()
    /// @brief      Getter for the default Border Width.
    /// @details    Get the default border width used in the non-client hit-tests.
    /// @return     The default border width value.
    int     DefaultBorderWidth()  const;


    /// @fn         int  SetBorderWidth( int iValue )
    /// @brief      Setter for the Border Width.
    /// @details    Set the border width used in the non-client hit-tests.
    /// @param      iValue      The value of the desired border width.
    void    SetBorderWidth( int iValue );


    /// @fn         int  ResetBorderWidth()
    /// @brief      Resetter for the Border Width.
    /// @details    Reset the default border width used in the non-client hit-tests.
    void    ResetBorderWidth();


public:
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- Public Blur API
    /// @fn         void  EnableBlurBehind()
    /// @brief      Enable the Blur Behind feature.
    /// @details    It uses different method according to the OS version and may lend different visual results.
    ///             Can be called at any time during the window lifetime.
    void    EnableBlurBehind();


    /// @fn         void  DisableBlurBehind()
    /// @brief      Disable the Blur Behind feature.
    /// @details    It uses different method according to the OS version and may lend different visual results.
    ///             Can be called at any time during the window lifetime.
    void    DisableBlurBehind();


private:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------ Private Win32 flags setup
    /// @fn         void  InitNativeFrameless()
    /// @brief      Init the frameless windows flags with aero snap behaviour.
    /// @details    Can be called only at construct time.
    void    InitNativeFrameless();

private:
//--------------------------------------------------------------------------------------
//------------------------------------------------- Private Non-Client OS event handling
    void  WM_NCHITTEST_Event_Handler( int iX, int iY, long *oResult );
    bool  NCHitLeftBorder(      const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitRightBorder(     const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitTopBorder(       const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitBottomBorder(    const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitTopLeftCorner(   const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitTopRightCorner(  const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitBotRightCorner(  const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitBotLeftCorner(   const  QRect&  iRect, const long iBorderWidth, long iX, long iY );

protected:
//--------------------------------------------------------------------------------------
//----------------------------------------------- Protected Non-Client OS event handling
    /// @fn         virtual  bool  NCHitCaption( const  QRect&  iRect, const long iBorderWidth, long iX, long iY )
    /// @brief      Overrideable Hit-Test to check if the cursor hits the caption.
    /// @details    A return value of true means the hit-test succeeded and the window can be moved and snaped natively.
    ///             A return value of false means the hit-test failed and the window will not respond to events.
    ///             You can use this to avoid moving the window if hitting a widget in the caption bar, like a button.
    /// @return     A boolean value, true: delegate native window events, false: abort native events.
    virtual  bool  NCHitCaption( const  QRect&  iRect, const long iBorderWidth, long iX, long iY );

protected:
//--------------------------------------------------------------------------------------
//------------------------------------------ Protected Qt / Win32 native events override
    /// @fn         virtual  bool  nativeEvent( const  QByteArray& eventType, void* message, long* result )  override
    /// @brief      overrided Qt's function native event, allowing for fine grained control over os non client events.
    /// @details    A return value of true means the event were handled in a custom way, and should stop propagate.
    ///             A return value of false means the event were not delivered and should propagate.
    ///             Call parent implementation if unsure.
    /// @return     A boolean value, true: stop event propagation, false: pursue event propagation.
    virtual  bool  nativeEvent( const  QByteArray& eventType, void* message, long* result )  override;

private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------- Private Data Members
    int  mBorderWidth;  ///< Non-Client OS border geometry width.

};

} // namespace  __private__
} // namespace  Rivet

