/**
*
*   Rivet
*__________________
*
* @file     Rivet.DefaultCaption.h
* @author   Clement Berthaud
* @brief    This file provides the declaration for the RDefaultCaption class.
*/
#pragma once
#include "Rivet/Rivet.CustomCaption.h"
#include "Rivet/Rivet.CustomButton.h"
#include <QObject>

namespace  Rivet {
/////////////////////////////////////////////////////
/// @class      RDefaultCaption
/// @brief      The RDefaultCaption class provides a default custom caption widget.
/// @details    It provides window snap / close / minimize / maximize mechanisms and buttons.
class  RDefaultCaption
    : public  RCustomCaption
{
    Q_OBJECT

private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Typedefs
    typedef  RDefaultCaption    tSelf;
    typedef  RCustomCaption     tSuperClass;


public:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
    /// @fn         virtual  ~RDefaultCaption()
    /// @brief      Default Destructor.
    /// @details    virtual, does nothing.
    virtual  ~RDefaultCaption();


    /// @fn         RDefaultCaption( QWidget* iParent = nullptr )
    /// @brief      Default Constructor.
    /// @details    Init data members.
    RDefaultCaption( QWidget* iParent = nullptr );


public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Public Geometry API
    QRect  ButtonsGeometry();


public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------- Public customization utilities
    void  ActiveBackgroundColor( const  QColor& iColor );
    const  QColor&  ActiveBackgroundColor();
    void  InactiveBackgroundColor( const  QColor& iColor );
    const  QColor&  InactiveBackgroundColor();
    void  SetBlackControls();
    void  SetWhiteControls();


protected:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt events override
    virtual  void  paintEvent(  QPaintEvent*    event )     override;
    virtual  void  resizeEvent( QResizeEvent*   event )     override;


protected:
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
    virtual void  Compose();


    /// @fn         void  Destroy()
    /// @brief      Delete GUI members.
    /// @details    Called only once by destructor.
    void  Destroy();


private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------- Private Data Members
    RCustomButton*  mCloseButton;       ///< The close button.
    RCustomButton*  mMaximizeButton;    ///< The maximize button.
    RCustomButton*  mMinimizeButton;    ///< The minimize button.
    QColor  mActiveBackgroundColor;     ///< The active caption color.
    QColor  mInactiveBackgroundColor;   ///< The inactive caption color.


};

} // namespace  Rivet

