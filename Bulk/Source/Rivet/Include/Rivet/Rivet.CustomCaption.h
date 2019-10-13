/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.CustomCaption.h
* 9-9-2018 18:56 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma once


#include <QObject>
#include <QWidget>


namespace  Rivet
{


class  CustomCaption :
    public QWidget
{

    Q_OBJECT

    typedef  QWidget            tSuperClass;
    typedef  CustomCaption      tSelf;

public:
    // Construction / Destruction
    virtual  ~CustomCaption();
    CustomCaption( QWidget* parent = Q_NULLPTR );

public:
    // Custom NC Handling implementation in caption
    virtual  bool  HitEmptySpace( long iX, long iY );

protected:
    // Protected Qt / WinAPI native events override
    virtual  bool  nativeEvent( const  QByteArray& eventType, void* message, long* result )  override;

public slots:
    // Private Qt Slots
    void  ProcessCloseClicked();
    void  ProcessMaximizeClicked();
    void  ProcessMinimizeClicked();
signals:
    // Private Qt Slots
    void  CloseClicked();
    void  MaximizeClicked();
    void  MinimizeClicked();

};


} // namespace  Rivet

