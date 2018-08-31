/*************************************************************************
*
*   Rivet
*__________________
*
* blur-behind-window.cpp
* 8-9-2018 21:10 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include <QApplication>


#include <RivetCustomMainWindow>
#include <RivetDefaultCaption>


int  main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    auto  w = new  ::Rivet::CustomMainWindow();
    auto  c = new  ::Rivet::DefaultCaption();
    auto  m = new  QWidget();
    m->setStyleSheet( "background-color:rgba( 0, 0, 0, 0.5 );" );
    w->SetCaptionWidget( c );
    w->SetCenterWidget( m );
    w->resize( 800, 600 );
    w->show();
    //w->EnableBlurBehind();

    return  app.exec();
}