/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.__private__.DockingManager.h
* 10-9-2018 00:10 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma once


#include <QObject>
#include <QRegion>
#include <QVector>

#include <functional>


// Forward Declarations
namespace  Rivet{ class  Tab; }
namespace  Rivet{ class  TabArea; }


namespace  Rivet
{
namespace  __private__
{


class cDockingManager :
    public  QObject
{
    Q_OBJECT

////////////////////////////////////////////////////////////////////////////////////////
////                            PRIVATE CONSTRUCTION                                ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Construction / Destruction
    virtual  ~cDockingManager();
    cDockingManager();

////////////////////////////////////////////////////////////////////////////////////////
////                            PUBLIC SINGLETON API                                ////
////////////////////////////////////////////////////////////////////////////////////////
public:
    // Singleton Accessor
    static  cDockingManager*  DockingManager();


////////////////////////////////////////////////////////////////////////////////////////
////                                INFO API                                        ////
////////////////////////////////////////////////////////////////////////////////////////
public:
    Tab*        CurrentDraggingTab()  const;
    TabArea*    CurrentTargetArea()  const;
    void        SetLastLiftedFrom( TabArea* iValue );
    TabArea*    GetLastLiftedFrom()  const;

////////////////////////////////////////////////////////////////////////////////////////
////                            REGISTER API                                        ////
////////////////////////////////////////////////////////////////////////////////////////
public:
    // Public Register API for Tabs & TabAreas
    void  RegisterTabArea(      TabArea* iTabArea );
    void  UnregisterTabArea(    TabArea* iTabArea );
    void  RegisterTab(          Tab* iTab );
    void  UnregisterTab(        Tab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                            PRIVATE SIGNAL SLOTS API                            ////
////////////////////////////////////////////////////////////////////////////////////////
private slots:
    // Docking Interface Slots
    void  TabLifted( Tab* iTab );
    void  TabDropped( Tab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE API                                     ////
////////////////////////////////////////////////////////////////////////////////////////
protected:
    // Qt Events overrides
    // Intercepting Tab Events
    virtual  bool  eventFilter( QObject* obj, QEvent* event)    override;

private:
    // Private Connection Interface
    void  InitConnectionsForTab( Tab* iTab );
    void  DestroyConnectionsForTab( Tab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE DATA                                    ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Private Data Members
    QVector< TabArea* >             mTabAreaList;
    Tab*                            mCurrentDraggingTab;
    TabArea*                        mCurrentTargetArea;
    TabArea*                        mLastLiftedFrom;
};


} // namespace  __private__


////////////////////////////////////////////////////////////////////////////////////////
////                            PUBLIC SINGLETON API                                ////
////////////////////////////////////////////////////////////////////////////////////////
// External Conveniency Singleton Accessor
::Rivet::__private__::cDockingManager*  DockingManager();


} // namespace  Rivet

