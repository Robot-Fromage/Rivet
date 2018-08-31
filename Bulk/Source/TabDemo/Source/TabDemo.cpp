/*************************************************************************
*
*   Rivet
*__________________
*
* browser-like-window.cpp
* 6-10-2018 21:49 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include <QApplication>

#include <QApplication>
#include <QPainter>
#include <QFocusEvent>
#include <QStackedWidget>
#include <QTextEdit>

#include <RivetCustomMainWindow>
#include <RivetDefaultCaption>
#include <RivetTabArea>
#include <RivetDockingManager>
#include <RivetDockingCallbackLibrary>


void  OnTabDroppedOutCB_OpenCustom( ::Rivet::Tab* iTab, ::Rivet::TabArea* iSrc );


class  cBrowserCaption :
    public  ::Rivet::DefaultCaption
{
    typedef  ::Rivet::DefaultCaption  tSuperClass;

public:
    virtual  ~cBrowserCaption() {
        Destroy();
    };

    cBrowserCaption( QWidget*  iParent = NULL ) :
        tSuperClass( iParent ),
        mTabArea( NULL )
    {
        Init();
        Build();
        Compose();
    };

    virtual  bool  HitEmptySpace( long iX, long iY ) {
        QPoint local( iX, iY );
        bool result = true;

        QList<QWidget*> list = this->findChildren<QWidget*>();

        // If a direct child contains our mouse coordinates, we refute.
        for(QWidget *w : list) {
            QRect g = w->geometry();
            if(g.contains( local ) && w->parent() == this )
                result = false;
        }

        // But if it is the tab area, we ask it for more details.
        if( result == false && mTabArea->geometry().contains( local ) )
            result = mTabArea->HitEmptySpace( local.x(), local.y() );

        return  result;
    }

    void  Init() {
        mTabArea = new  ::Rivet::TabArea( this );
    }

    void  Build() {
        mTabArea->SetMaximumTabWidth( 50 );
        mTabArea->SetOverlap( 20 );
        mTabArea->SetTabsClosable( true );
    }

    virtual  void  Compose() override {
        tSuperClass::Compose();
        mTabArea->move( height(), 0 );
        mTabArea->resize( ButtonsGeometry().x() - height()*2, height() );
    }

    void  Destroy() {
        delete  mTabArea;
    }

    ::Rivet::TabArea* TabArea() {
        return  mTabArea;
    }

private:
    ::Rivet::TabArea* mTabArea;
};


int  main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    auto  w = new  ::Rivet::CustomMainWindow();
    auto  c = new  cBrowserCaption();
    auto  s = new  QStackedWidget();

    s->setAttribute( Qt::WA_DeleteOnClose );
    c->setAttribute( Qt::WA_DeleteOnClose );
    w->setAttribute( Qt::WA_DeleteOnClose );

    c->ActiveBackgroundColor( QColor( 220, 220, 220 ) );
    c->InactiveBackgroundColor( QColor( 200, 200, 200 ) );
    c->SetBlackControls();

    c->TabArea()->SetBlackControls();
    c->TabArea()->EnableBlurEffectControls( false );
    c->TabArea()->SetOnAreaBecomesEmptyCB( ::Rivet::OnAreaBecomesEmptyCB_DoNothing );
    c->TabArea()->SetOnTabDroppedOutCB( OnTabDroppedOutCB_OpenCustom );
    c->TabArea()->SetLinkedStack( s );

    w->setMinimumWidth( 300 );
    w->SetCaptionWidget( c );
    w->SetCenterWidget( s );
    w->SetCaptionHeight( 20 );
    w->resize( 800, 600 );
    w->show();

    for( int i = 0; i < 5; ++i )
    {
        auto t = new ::Rivet::Tab();
        t->SetColor( QColor( 255, 255, 255 ) );
        t->SetFadeColor( QColor( 180, 180, 180 ) );
        //auto m = new  ::Rivet::MonacoWidget();
        auto m = new  QTextEdit();
        m->setAttribute( Qt::WA_DeleteOnClose );
        t->SetLinkWidget( m );
        t->SetTitle( QString::number( i ) );

        c->TabArea()->ManualAddNewTab( t );
    }

    return  app.exec();
}


void
OnTabDroppedOutCB_OpenCustom( ::Rivet::Tab* iTab, ::Rivet::TabArea* iSrc )
{
    auto  w = new  ::Rivet::CustomMainWindow();
    auto  c = new  cBrowserCaption();
    auto  s = new  QStackedWidget();

    s->setAttribute( Qt::WA_DeleteOnClose );
    c->setAttribute( Qt::WA_DeleteOnClose );
    w->setAttribute( Qt::WA_DeleteOnClose );

    c->ActiveBackgroundColor( QColor( 220, 220, 220 ) );
    c->InactiveBackgroundColor( QColor( 200, 200, 200 ) );
    c->SetBlackControls();

    c->TabArea()->SetBlackControls();
    c->TabArea()->EnableBlurEffectControls( false );
    c->TabArea()->SetOnAreaBecomesEmptyCB( ::Rivet::OnAreaBecomesEmptyCB_CloseTopLevel );
    c->TabArea()->SetOnTabDroppedOutCB( OnTabDroppedOutCB_OpenCustom );
    c->TabArea()->SetLinkedStack( s );
    iSrc->GetLinkedStack()->removeWidget( iTab->GetLinkWidget() );
    c->TabArea()->DockHere(iTab);

    w->setMinimumWidth( 300 );
    w->SetCaptionWidget( c );
    w->SetCenterWidget( s );
    w->SetCaptionHeight( 20 );
    w->resize( 800, 600 );

    w->move(iTab->pos());
    w->setFocus();
    w->show();
    w->raise();


    // Now this is optional if we want to activate the window once the tab has been docked
    // the set focus method just doesn't work
    // This does activate & raise the window but doesn't grab the focus
    w->raise();  // for MacOS
    w->activateWindow(); // for Windows

    // the event object is released then in event loop (?)
    QFocusEvent* eventFocus = new QFocusEvent(QEvent::FocusIn);
    // posting event for forcing the focus with low priority
    qApp->postEvent( w, (QEvent *)eventFocus, Qt::LowEventPriority );
}

