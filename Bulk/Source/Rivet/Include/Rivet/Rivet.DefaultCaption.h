/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.DefaultCaption.h
* 9-9-2018 18:56 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma once


#include <QObject>


#include "Rivet/Rivet.CustomCaption.h"
#include "Rivet/Rivet.CustomButton.h"


namespace  Rivet
{


class  DefaultCaption :
    public  CustomCaption
{

    Q_OBJECT

    typedef  CustomCaption  tSuperClass;

public:
    // Construction / Destruction
    virtual  ~DefaultCaption();
    DefaultCaption( QWidget* parent = Q_NULLPTR );

public:
    // Geometry public API
    QRect  ButtonsGeometry();

public:
    // Public customization utilities
    void  ActiveBackgroundColor( const  QColor& iColor );
    const  QColor&  ActiveBackgroundColor();
    void  InactiveBackgroundColor( const  QColor& iColor );
    const  QColor&  InactiveBackgroundColor();
    void  SetBlackControls();
    void  SetWhiteControls();

protected:
    // Protected Qt event overrides
    virtual  void  paintEvent(  QPaintEvent*    event )     override;
    virtual  void  resizeEvent( QResizeEvent*   event )     override;

private:
    // GUI utilities
private: void  Init();
private: void  Build();
protected: virtual  void  Compose();
private: void  Destroy();

private:
    // Private Data Members
    RCustomButton*  mCloseButton;
    RCustomButton*  mMaximizeButton;
    RCustomButton*  mMinimizeButton;

    QColor  mActiveBackgroundColor;
    QColor  mInactiveBackgroundColor;
};


} // namespace  Rivet

