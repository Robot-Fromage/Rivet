/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.CustomMainWindow.h
*
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma once


#include <QWidget>


#include "Rivet/__private__/Rivet.__private__.CustomMainWindowBase.h"
#include "Rivet/Rivet.CustomCaption.h"


// Tertiary level for custom windows
// Handles Custom events and provides Custom API calls
// You can derive this class if you want more customization


namespace  Rivet
{


class  CustomMainWindow :
    public ::Rivet::__private__::cCustomMainWindowBase
{
    Q_OBJECT

    typedef  ::Rivet::__private__::cCustomMainWindowBase    tSuperClass;
    typedef  CustomMainWindow                               tSelf;

public:
    // Construction / Destruction
    virtual  ~CustomMainWindow();
    CustomMainWindow( QWidget* parent = Q_NULLPTR );

public:
    // Center / Caption API
    CustomCaption*  CaptionWidget();  // Return pointer to mCaptionWidget, this keeps owning it
    void  SetCaptionWidget( CustomCaption* iCaptionWidget );  // Pass pointer to mCaptionWidget, becoming owner
    //-
    QWidget* CenterWidget();  // Return pointer to mCenterWidget, keeps owning it.
    void  SetCenterWidget( QWidget* iCenterWidget ); // Acquire ownership
    void  Recompose();

protected:
    // Protected Non-Client OS behaviour handling overrides
    virtual  bool  NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )  override;

protected:
    // Protected Qt events override
    virtual  void  resizeEvent( QResizeEvent*  event )  override;

private  slots:
    // Private Qt Slots
    void  ProcessCloseClicked();
    void  ProcessMaximizeClicked();
    void  ProcessMinimizeClicked();

private:
    // Internal GUI management
    void  Init();       // allocate GUI members
    void  Build();      // set GUI members flags, styles, properties
    void  Compose();    // compose members in GUI
    void  Destroy();    // delete GUI members

private:
    CustomCaption* mCaptionWidget;
    QWidget* mCenterWidget;

};


} // namespace  Rivet

