/**
*
*   Rivet
*__________________
*
* @file     Rivet.CustomCaption.h
* @author   Clement Berthaud
* @brief    This file provides the declaration for the RCustomCaption class.
*/
#pragma once
#include <QObject>
#include <QWidget>

namespace  Rivet {
/////////////////////////////////////////////////////
/// @class      RCustomCaption
/// @brief      The RCustomCaption class provides a base framework for custom caption widgets.
/// @details    RCustomCaption must have a HitEmptySpace method, override this for custom behaviours.
///             The default implementation returns false if it hits any of the child widgets.
///             Children can be set externally.
///             Parent Custom window will listen to the signals.
class  RCustomCaption
    : public QWidget
{
    Q_OBJECT

private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Typedefs
    typedef  RCustomCaption     tSelf;
    typedef  QWidget            tSuperClass;

public:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
    /// @fn         virtual  ~RCustomCaption()
    /// @brief      Default Destructor.
    /// @details    virtual, does nothing.
    virtual  ~RCustomCaption();


    /// @fn         RCustomCaption( QWidget *parent = nullptr )
    /// @brief      Default Constructor.
    /// @details    Init member data with default values.
    RCustomCaption( QWidget* iParent = nullptr );


public:
//--------------------------------------------------------------------------------------
//----------------------------------------- Custom NC Handling implementation in caption
    /// @fn         virtual  bool  HitEmptySpace( long iX, long iY )
    /// @brief      Check if the cursor hit empty space to allow for NC events.
    /// @details    Handles internal geometry tests.
    /// @param      iX      the x coordinate in parent space.
    /// @param      iY      the y coordinate in parent space.
    /// @return     A boolean flag stating if the hit succeeded or not.
    virtual  bool  HitEmptySpace( long iX, long iY );


protected:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
    virtual  bool  nativeEvent( const  QByteArray& eventType, void* message, long* result )  override;


public slots:
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- Public Qt Slots
    void  ProcessCloseClicked();
    void  ProcessMaximizeClicked();
    void  ProcessMinimizeClicked();


signals:
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Public Qt Signals
    void  CloseClicked();
    void  MaximizeClicked();
    void  MinimizeClicked();


};


} // namespace  Rivet

