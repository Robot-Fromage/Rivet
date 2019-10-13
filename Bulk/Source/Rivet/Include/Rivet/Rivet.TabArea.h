/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.TabArea.h
* 6-10-2018 21:50 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma once


#include <QWidget>


#include "Rivet/Rivet.Tab.h"


// Rivet Forward declarations
namespace  Rivet  { class  RCustomButton; }


// Qt Forward declarations
class  QGraphicsDropShadowEffect;
class  QScrollArea;
class  QScroller;
class  QTimer;
class  QStackedWidget;

namespace  Rivet
{

typedef void (*OnAreaBecomesEmptyCB)( TabArea* self );

class TabArea :
    public QWidget
{
    Q_OBJECT

    typedef  QWidget  tSuperClass;

////////////////////////////////////////////////////////////////////////////////////////
////                                PUBLIC API                                      ////
////////////////////////////////////////////////////////////////////////////////////////
public:
    // Construction / Destruction
    virtual         ~TabArea();
                    TabArea( QWidget *parent = Q_NULLPTR );

public:
    // Custom Hit Test Handling implementation in TabArea
    bool            HitEmptySpace( long iX, long iY );

public:
    // Tab Control Interface
    void            ManualAddNewTab(    Tab* iTab );
    void            DockHere(           Tab* iTab);
    void            SetCurrentTab(      Tab* iTab );
    int             NTabs()  const;
    void            SetCandidateTab( Tab* iTab );
    void            SetNoCandidateTab();

    void            SetLinkedStack( QStackedWidget* iStack );
    QStackedWidget* GetLinkedStack()  const;

public:
    // Tab-Specific Geometry Ordering
    void            Reorder();
    void            Recompose();

public:
    // Overlap Size Style accessors
    void            SetOverlap( int iValue );
    void            SetMaximumTabWidth( int iWidth );
    void            SetMinimumTabWidth( int iWidth );
    void            SetTabsShapeStyle( Tab::eTabShape iValue );
    void            SetTabsClosable( bool iValue );
    void            SetTabsLiftable( bool iValue );

    void            SetOnAreaBecomesEmptyCB( OnAreaBecomesEmptyCB iOnAreaBecomesEmptyCB );
    void            SetOnTabDroppedOutCB( OnTabDroppedOutCB iOnTabDroppedOutCB );

    int                 GetOverlap()            const;
    int                 GetMaximumTabWidth()    const;
    int                 GetMinimumTabWidth()    const;
    int                 GetTabWidth()           const;
    Tab::eTabShape    GetTabsShapeStyle()     const;
    bool                GetTabsClosable()       const;
    bool                GetTabsLiftable()       const;

    OnAreaBecomesEmptyCB    GetOnAreaBecomesEmptyCB()  const;
    OnTabDroppedOutCB       GetOnTabDroppedOutCB()  const;

    void  SetBlackControls();
    void  SetWhiteControls();

    void  EnableBlurEffectControls( bool iValue );

public:
    // ID tag Interface
    void             SetTag( const  QString&  iTag );
    const  QString&  GetTag()  const;


////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE API                                     ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Private Tab Composing & Style utilities
    void            SetTabWidth( int iWidth );
    int             ComputeSmartWidthWithOverlap()  const;
    void            RecomputeCurrentTabOnImminentChange();
    void            TakeActionOnEmpty();

protected:
    // Qt Events overrides
    virtual  void   resizeEvent( QResizeEvent* event )          override;
    virtual  void   wheelEvent( QWheelEvent* event )            override;

private:
    // Private GUI Processing Functions
    void  Init();
    void  Build();
    void  Compose();
    void  Destroy();

private:
    // Internal Private Utilities on Compose
    void  ComposeScrollArea();

private:
    // Docking Manager Registering API
    void  Register();
    void  Unregister();

////////////////////////////////////////////////////////////////////////////////////////
////                              SIGNAL SLOTS API                                  ////
////////////////////////////////////////////////////////////////////////////////////////
public slots:
    // Debug Slots
    //void  _NewTab();        // Debug

private slots:
    // Dragging Slots
    void  DomesticTabLifted(     Tab* iTab );
    void  DomesticTabDropped(    Tab* iTab );
    void  ForeignTabDropped(    Tab* iTab );

    // Navigation Slots
    void  NavDelta( int iDelta, int iTimeMS );

    void  NavLeftDoubleClick();
    void  NavLeftDown();
    void  NavLeftWork();
    void  NavLeftUp();

    void  NavRightDoubleClick();
    void  NavRightDown();
    void  NavRightWork();
    void  NavRightUp();

    void  ProcessCandidateDragHover();

    // Tab Control Slots
    void  DomesticTabSelected(   Tab* iTab );
    void  CloseCurrentTab();
    void  SwitchToNextTab();
    void  CloseTab( Tab* iTab );

public slots:signals:
    void  AreaEmpty( TabArea* iArea );
    void  CurrentTabChanged( Tab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE DATA                                    ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Private Data Members
    // Owned Tabs OBjects Data
    QVector< Tab* >            mDomesticTabs;

    // Scroll Related Widgets Data
    QScrollArea*                mScrollArea;
    QWidget*                    mScrollWidgetWrapper;
    QScroller*                  mScroller;
    RCustomButton*               mLeftButton;
    RCustomButton*               mRightButton;

    // Size data
    int mTabWidth;
    int mMaximumTabWidth;
    int mMinimumTabWidth;
    int mOverlap;

    // Style Data
    QGraphicsDropShadowEffect*  mLeftDropShadowEffect;
    QGraphicsDropShadowEffect*  mRightDropShadowEffect;
    Tab::eTabShape            mTabsShapeStyle;
    bool                        mTabsClosable;
    bool                        mTabsLiftable;

    // State Data
    Tab*                        mCurrentTab; // Active
    Tab*                        mCandidateTab; // Dragging

    // Nav Data
    QTimer*                     mNavTimerLeft;
    QTimer*                     mNavTimerRight;
    QTimer*                     mProcessCandidateTimer;

    // ID Data
    QString                     mTag;

    OnAreaBecomesEmptyCB        mOnAreaBecomesEmptyCB;
    OnTabDroppedOutCB           mOnTabDroppedOutCB;
    QStackedWidget*             mLinkedStack;
};


} // namespace Rivet

