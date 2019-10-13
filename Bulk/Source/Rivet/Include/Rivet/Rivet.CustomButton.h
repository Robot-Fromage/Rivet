/**
*
*   Rivet
*__________________
*
* @file     Rivet.CustomButton.h
* @author   Clement Berthaud
* @brief    This file provides the declaration for the RCustomButton class.
*/
#pragma once
#include <QObject>
#include <QPushButton>

namespace  Rivet {
/////////////////////////////////////////////////////
/// @class      RCustomButton
/// @brief      The RCustomButton class provides a custom window with a vector shape.
/// @details    No resources are needed for drawing the button.
///             It handles a limited set of shapes, see \e RCustomButton::eButtonIconShape.
class  RCustomButton
    : public  QPushButton
{
    Q_OBJECT

private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Typedefs
    typedef  RCustomButton  tSelf;
    typedef  QPushButton    tSuperClass;


public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- Public Enums
/// @enum   eButtonIconShape
/// @brief  Enum for Custom Button Icon Shape
enum  class  eButtonIconShape
{
      kNone         ///< None, No shape, this is a valid state.
    , kClose        ///< Close, Cross shape for close window button.
    , kMaximize     ///< Maximize, Square shape for maximize window button.
    , kMinimize     ///< Minimize, Minus shape for minimize window button.
    , kLeftArrow    ///< LeftArrow, Triangle shape pointing left for TabArea.
    , kRightArrow   ///< RightArrow, Triangle shape pointing right for TabArea.
};


/// @enum   eButtonBackgroundShape
/// @brief  Enum for Custom Button Background Shape
enum  class  eButtonBackgroundShape
{
      kSquare       ///< Square, full size square or rectangle background.
    , kDisk         ///< Disk, disk or ellipse shaped background.
};


private:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ Private Enums
/// @enum   eButtonState
/// @brief  Enum for Custom Button state.
enum  class  eButtonState
{
    kNone           = 0,    ///< None, The button is in idle state.
    kHovered        = 1,    ///< Hovered, The button is in hovered by the mouse.
    kPressed        = 2,    ///< Pressed, The button is in pressed but the mouse does not hover.
    kPressedHovered = 3,    ///< PressedHovered, The button is in pressed and hovered by the mouse.
};


public:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
    /// @fn         virtual  ~RCustomButton()
    /// @brief      Default Destructor.
    /// @details    virtual, does nothing.
    virtual  ~RCustomButton();


    /// @fn         RCustomButton( QWidget *parent = nullptr )
    /// @brief      Default Constructor.
    /// @details    Init member data with default values.
    RCustomButton( QWidget* iParent = nullptr );

public:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Public Style Setup Accessors
    void    SetBackgroundColor( const  QColor&  iColor );
    void    SetHoveredBackgroundColor( const  QColor&  iColor );
    void    SetPressedBackgroundColor( const  QColor&  iColor );
    const   QColor&  GetBackgroundColor()  const;
    const   QColor&  GetHoveredBackgroundColor()  const;
    const   QColor&  GetPressedBackgroundColor()  const;
    void    SetColor( const  QColor&  iColor );
    void    SetHoveredColor( const  QColor&  iColor );
    void    SetPressedColor( const  QColor&  iColor );
    const   QColor&  GetColor()  const;
    const   QColor&  GetHoveredColor()  const;
    const   QColor&  GetPressedColor()  const;
    void    SetIconShape( const  eButtonIconShape&  iShape );
    const   eButtonIconShape&  GetShape()  const;
    void    SetBackgroundShape( const  eButtonBackgroundShape&  iShape );
    const   eButtonBackgroundShape&  GetBackgroundShape()  const;
    void    SetIconSize( int iSize );
    int     GetIconSize()  const;


public:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Public State Accessors
    bool IsHovered();
    bool IsPressed();


public slots:signals:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- Public Signals
    void  DoubleClicked();


protected:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
    virtual  void  enterEvent(              QEvent*         event )  override;
    virtual  void  leaveEvent(              QEvent*         event )  override;
    virtual  void  mousePressEvent(         QMouseEvent*    event )  override;
    virtual  void  mouseReleaseEvent(       QMouseEvent*    event )  override;
    virtual  void  mouseDoubleClickEvent(   QMouseEvent*    event )  override;
    virtual  void  paintEvent(              QPaintEvent*    event )  override;


private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------- Private Data Members
    bool                    mHovered;               ///< Boolean flag, wether the button is hovered or not.
    bool                    mPressed;               ///< Boolean flag, wether the button is pressed or not.
    QColor                  mBgColor;               ///< The idle background color.
    QColor                  mHoveredBgColor;        ///< The hovered background color.
    QColor                  mPressedBgColor;        ///< The pressed background color.
    QColor                  mPressedHoveredBgColor; ///< The pressed-hovered background color.
    QColor                  mColor;                 ///< The idle color.
    QColor                  mHoveredColor;          ///< The hovered color.
    QColor                  mPressedColor;          ///< The pressed color.
    QColor                  mPressedHoveredColor;   ///< The pressed-hovered color.
    eButtonIconShape        mIconShape;             ///< The icon shape.
    eButtonBackgroundShape  mBackgroundShape;       ///< The background shape.
    int                     mIconSize;              ///< The icon size.

};


} // namespace  Rivet

