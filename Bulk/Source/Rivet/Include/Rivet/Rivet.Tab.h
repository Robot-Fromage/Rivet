/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.Tab.h
* 2-10-2018 17:41 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma once


#include <QColor>
#include <QPoint>
#include <QRect>
#include <String>
#include <QWidget>


/* Rivet Forward declarations */
namespace  Rivet  {  class  TabArea;  }
namespace  Rivet  {  class  CustomButton;  }


/* Qt Forward declarations */
class  QGraphicsDropShadowEffect;
class  QGraphicsOpacityEffect;
class  QLabel;
class  QPropertyAnimation;
class  QPushButton;


namespace  Rivet
{

class Tab;
typedef void (*OnTabDroppedOutCB)( Tab* self, TabArea* src );


class  Tab :
    public  QWidget
{
    Q_OBJECT

    typedef  QWidget  tSuperClass;

////////////////////////////////////////////////////////////////////////////////////////
////                                PUBLIC API                                      ////
////////////////////////////////////////////////////////////////////////////////////////
public:
    // Shape Style Enum
enum class eShapeStyle
{
    kLine,
    kBezier,
    kRect,
    kRect_Line,
};

public:
    // Construction / Destruction
    virtual  ~Tab();
    Tab( QWidget*  parent = NULL );

public:
    // Docking Interface Accessors
    bool            Docked()            const;
    bool            Dragging()          const;
    const  QPoint&  DragShift()         const;

    void            FinishDrag();

public:
    // Animation Interface
    void            SetAnimatedMovement( const  QPoint& iDest );
    void            StopAnimatedMovement();

    const  QRect&   TargetGeometry()     const;

public:
    // Tab ColorStyle Interface Accessors
    void            SetTitle( const  QString&  iTitle );
    void            SetShapeStyle( eShapeStyle iTabStyle );
    void            SetColor( const  QColor&  iColor );
    void            SetFadeColor( const  QColor& iColor );

    QString         GetTitle()          const;
    QString         GetTitleEllided()   const;
    eShapeStyle     GetTabStyle()       const;
    const  QColor&  GetColor()          const;
    const  QColor&  GetFadeColor()      const;

public:
    // Tab State accessors
    void            SetActive( bool iValue = true );

    bool            IsHovered()         const;
    bool            IsPressed()         const;
    bool            IsActive()          const;

public:
    // Behaviour constraints
    void            SetLiftable( bool iValue = true );
    void            SetClosable( bool iValue = true );

    bool            IsLiftable()        const;
    bool            IsClosable()        const;

public:
    // ID tag Interface
    void             SetTag( const  QString&  iTag );
    const  QString&  GetTag()  const;

public:
    // CB API
    void                SetOnTabDroppedOutCB( OnTabDroppedOutCB iOnTabDroppedOutCB );
    OnTabDroppedOutCB   GetOnTabDroppedOutCB()  const;

public:
    // Link API
    void                SetLinkWidget( QWidget* iWidget );
    QWidget*            GetLinkWidget()  const;

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE API                                     ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Qt Events overrides
    virtual  void   resizeEvent(        QResizeEvent*   event )     override;
    virtual  void   enterEvent(         QEvent*         event )     override;
    virtual  void   leaveEvent(         QEvent*         event )     override;
    virtual  void   mousePressEvent(    QMouseEvent*    event )     override;
    virtual  void   mouseMoveEvent(     QMouseEvent*    event )     override;
    virtual  void   mouseReleaseEvent(  QMouseEvent*    event )     override;
    virtual  void   paintEvent(         QPaintEvent*    event )     override;
    virtual  void   closeEvent(         QCloseEvent*    event )     override;

private:
    // Private GUI Processing Functions
    void            Init();
    void            Build();
    void            Compose();
    void            Destroy();

private:
    // Internal Tab Utilities on Compose
    void            CheckTitleEllipsis();

private:
    // Docking Manager Registering API
    void            Register();
    void            Unregister();


////////////////////////////////////////////////////////////////////////////////////////
////                              SIGNAL SLOTS API                                  ////
////////////////////////////////////////////////////////////////////////////////////////
public slots:
    // Tab Slots
    void  ProcessCloseClicked();

signals:
    // Docking Interface Signals
    void  Lifted( Tab* );
    void  Dropped( Tab* );
    void  Selected( Tab* );
    void  CloseClicked( Tab* );

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE DATA                                    ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Private Data Members
    QLabel*                     mTitleLabel;
    CustomButton*               mCloseButton;
    QWidget*                    mInvisibleBackgroundRect;

    QString                     mTitleText;

    // Drag Data
    QPoint                      mDragShift;
    bool                        mDragging;
    bool                        mDragEnabled;

    // Animation Data
    QPropertyAnimation*         mAnimation;
    QRect                       mAnimationTargetGeometry;

    // Style Data
    eShapeStyle                 mTabStyle;
    QColor                      mBaseColor;
    QColor                      mFadeColor;
    int                         mSlopePadding;
    QGraphicsDropShadowEffect*  mTitleDropShadowEffect;
    QGraphicsDropShadowEffect*  mGlobalDropShadowEffect;
    QColor                      mTextColor;
    QColor                      mTitleDropShadowColor;
    int                         mDropShadowShift;
    QGraphicsOpacityEffect*     mOpacityEffect;

    //Behaviour constraints
    bool                        mLiftable;
    bool                        mClosable;
    QString                     mTag;

    // State Data
    bool                        mHovered;
    bool                        mPressed;
    bool                        mActive;

    // CB Data
    OnTabDroppedOutCB           mOnTabDroppedOutCB;
    QWidget*                    mLinkWidget;
};

} // namespace  Rivet

