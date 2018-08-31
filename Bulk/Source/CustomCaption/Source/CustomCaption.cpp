/*************************************************************************
*
*   Rivet
*__________________
*
* custom-caption.cpp
* 24-10-2018 19:15 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include <QApplication>
#include <QPainter>
#include <QPushButton>


#include <RivetCustomMainWindow>
#include <RivetCustomCaption>


class  cMyCustomCaption :
    public  ::Rivet::CustomCaption
{
    typedef  ::Rivet::CustomCaption  tSuperClass;

public:
    virtual  ~cMyCustomCaption()
    {
        for( auto item : mBtns )  delete item;
    };


    cMyCustomCaption( QWidget*  iParent = NULL ):tSuperClass( iParent )
    {
        for( int i = 0; i < 10; ++i )
        {
            mBtns.push_back( new  QPushButton( this ) );
            mBtns.last()->setText( QString( '0' + char( i ) ) );
            int s = 20;
            mBtns.last()->resize( s, s );
            mBtns.last()->move( i * s, 0 );
            QObject::connect( mBtns.last(), SIGNAL( clicked() ), this, SLOT( tSuperClass::ProcessCloseClicked() ) );
        }
    };

protected:
    virtual  void  paintEvent(  QPaintEvent*  event )  override
    {
        QPainter painter(this);
        painter.setBrush( QColor( 127, 100, 20 ) );
        painter.setPen( Qt::NoPen );
        painter.drawRect( 0, 0, width(), height() );
    };

private:
    QVector< QPushButton* > mBtns;
};


int  main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    auto  w = new  ::Rivet::CustomMainWindow();
    auto  c = new  cMyCustomCaption();
    w->SetCaptionWidget( c );
    w->resize( 800, 600 );
    w->show();

    return  app.exec();
}