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
namespace  Rivet{ class  RTab; }
namespace  Rivet{ class  RTabArea; }


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
    RTab*        CurrentDraggingTab()  const;
    RTabArea*    CurrentTargetArea()  const;
    void        SetLastLiftedFrom( RTabArea* iValue );
    RTabArea*    GetLastLiftedFrom()  const;

////////////////////////////////////////////////////////////////////////////////////////
////                            REGISTER API                                        ////
////////////////////////////////////////////////////////////////////////////////////////
public:
    // Public Register API for Tabs & TabAreas
    void  RegisterTabArea(      RTabArea* iTabArea );
    void  UnregisterTabArea(    RTabArea* iTabArea );
    void  RegisterTab(          RTab* iTab );
    void  UnregisterTab(        RTab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                            PRIVATE SIGNAL SLOTS API                            ////
////////////////////////////////////////////////////////////////////////////////////////
private slots:
    // Docking Interface Slots
    void  TabLifted( RTab* iTab );
    void  TabDropped( RTab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE API                                     ////
////////////////////////////////////////////////////////////////////////////////////////
protected:
    // Qt Events overrides
    // Intercepting Tab Events
    virtual  bool  eventFilter( QObject* obj, QEvent* event)    override;

private:
    // Private Connection Interface
    void  InitConnectionsForTab( RTab* iTab );
    void  DestroyConnectionsForTab( RTab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE DATA                                    ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Private Data Members
    QVector< RTabArea* >             mTabAreaList;
    RTab*                            mCurrentDraggingTab;
    RTabArea*                        mCurrentTargetArea;
    RTabArea*                        mLastLiftedFrom;
};


} // namespace  __private__


////////////////////////////////////////////////////////////////////////////////////////
////                            PUBLIC SINGLETON API                                ////
////////////////////////////////////////////////////////////////////////////////////////
// External Conveniency Singleton Accessor
::Rivet::__private__::cDockingManager*  DockingManager();


} // namespace  Rivet

