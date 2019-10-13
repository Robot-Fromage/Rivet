/**
*
*   Rivet
*__________________
*
* @file     Rivet.CustomMainWindow.h
* @author   Clement Berthaud
* @brief    This file provides the declaration for the RCustomWindow class.
*/
#pragma once
#include "Rivet/__private__/Rivet.__private__.CustomMainWindowBase.h"
#include "Rivet/Rivet.CustomCaption.h"
#include <QWidget>

namespace  Rivet {
/////////////////////////////////////////////////////
/// @class      RCustomMainWindow
/// @brief      The RCustomMainWindow class provides a custom window widget.
/// @details    Tertiary level for custom windows.
///             Handles Custom events and provides Custom API calls
///             Users can derive this class for more customization
class  RCustomMainWindow
    : public ::Rivet::__private__::RCustomMainWindowBase
{
    Q_OBJECT

private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Typedefs
    typedef  RCustomMainWindow                              tSelf;
    typedef  ::Rivet::__private__::RCustomMainWindowBase    tSuperClass;

public:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
    /// @fn         virtual  ~RCustomMainWindow()
    /// @brief      Default Destructor.
    /// @details    virtual, does nothing.
    virtual  ~RCustomMainWindow();


    /// @fn         RCustomMainWindow( QWidget* iParent = nullptr )
    /// @brief      Default Constructor.
    /// @details    Init data members.
    RCustomMainWindow( QWidget* iParent = nullptr );


public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Center Caption API
    /// @fn         RCustomCaption*  CaptionWidget()  const
    /// @brief      Getter for custom caption widget
    /// @return     A pointer to the owned member mCaptionWidget, can be null.
    RCustomCaption*     CaptionWidget()  const;


    /// @fn         QWidget*  CenterWidget()  const
    /// @brief      Getter for custom center widget
    /// @return     A pointer to the owned  member mCenterWidget, can be null.
    QWidget*            CenterWidget()  const;


    /// @fn         void  SetCaptionWidget( RCustomCaption* iCaptionWidget )
    /// @brief      Setter for the custom caption widget.
    /// @param      iCaptionWidget      The RCustomCaption* to take ownership over and place in the window.
    /// @details    The object takes ownership over the passed parameter. Can be null.
    void                SetCaptionWidget( RCustomCaption* iCaptionWidget );


    /// @fn         void  SetCenterWidget( QWidget* iCenterWidget )
    /// @brief      Setter for the custom center widget.
    /// @param      iCenterWidget       The QWidget* to take ownership over and place in the window.
    /// @details    The object takes ownership over the passed parameter. Can be null.
    void                SetCenterWidget( QWidget* iCenterWidget );


    /// @fn         void  Recompose()
    /// @brief      Recompose internal geometry.
    /// @details    Direct call to private Compose method.
    void                Recompose();


//--------------------------------------------------------------------------------------
//----------------------------------------------- Protected Non-Client OS event handling
    /// @fn         virtual  bool  NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )  override
    /// @brief      Override of the Hit-Test to check if the cursor hits the caption.
    /// @details    This implementation delegates the call to the caption widget if elligible.
    /// @return     A boolean value stating the behaviour to take on caption events.
    virtual  bool  NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )  override;


protected:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt events override
    virtual  void  resizeEvent( QResizeEvent*  event )  override;


private  slots:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------- Private Qt Slots
    void  ProcessCloseClicked();
    void  ProcessMaximizeClicked();
    void  ProcessMinimizeClicked();


private:
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------- Internal GUI management
    /// @fn         void  Init()
    /// @brief      Init GUI members.
    /// @details    Called only once by constructor.
    void  Init();

    /// @fn         void  Build()
    /// @brief      Setup GUI members flags, styles, properties.
    /// @details    Called only once by constructor.
    void  Build();


    /// @fn         void  Compose()
    /// @brief      Compose geometry of members in GUI.
    /// @details    Called repeatedly on resize or recompose.
    void  Compose();


    /// @fn         void  Destroy()
    /// @brief      Delete GUI members.
    /// @details    Called only once by destructor.
    void  Destroy();


private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------- Private Data Members
    RCustomCaption* mCaptionWidget; ///< The RCustomCaption widget, owned but set via the accessors, can be null.
    QWidget*        mCenterWidget;  ///< The center QWidget, owned but set via the accessors, can be null.


};


} // namespace  Rivet

