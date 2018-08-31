/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.MonacoWidget.h
* 9-9-2018 10:38 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma  once


class  QWebEngineView;
class  QWebEnginePage;
#include <QWidget>


#include  "Rivet/__private__/Rivet.__private__.ASyncJS_PPFramework.h"


namespace  Rivet
{


void SetMonacoPrefix( const  QString&  iPrefix );
QString GetMonacoPrefix();
QString GetFullMonacoPath();


class  MonacoWidget :
    public  QWidget
{
    Q_OBJECT

    typedef  QWidget  tSuperClass;

public:
    // Construction / Destruction
    MonacoWidget( QWidget* iParent = NULL );
    virtual  ~MonacoWidget();

public slots:
    // Editor API Exec
    Q_SLOT void     Layout(); // Recompute size of the editor in embedded page
    Q_SLOT QString  GetText();
    Q_SLOT void     SetText( const  QString&  iStr );
    Q_SLOT void     ClearText();
    Q_SLOT void     AppendText( const  QString&  iStr );

    Q_SLOT QString  GetLanguage();
    Q_SLOT void     SetLanguage( const  QString&  iStr );


protected:
    // Protected Qt events override
    virtual  void  resizeEvent( QResizeEvent*  event )  override;

private:
    // Private Internal GUI Utilitites
    void  Init();
    void  Build();
    void  Compose();
    void  Destroy();

private:
    // Private Data Members
    QWebEngineView* mEmbeddedBrowser;

private:
    // Async JS Framework
    Rivet_ASyncJS_Decl

};


} // namespace  Rivet

