/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.DockingCallbackLibrary.cpp
* 22-9-2018 16:02 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include "Rivet/Rivet.DockingCallbackLibrary.h"


#include "Rivet/Rivet.TabArea.h"


namespace  Rivet
{


void
OnAreaBecomesEmptyCB_CloseTopLevel( TabArea* iArea )
{
    auto tw = iArea->topLevelWidget();
    tw->close();
    tw->deleteLater();
}


void
OnAreaBecomesEmptyCB_DoNothing( TabArea* iArea )
{
}


void
OnTabDroppedOutCB_RevertBack( Tab* iTab, TabArea* iSrc )
{
    iSrc->DockHere( iTab );
}


void
OnTabDroppedOutCB_Open( Tab* iTab, TabArea* iSrc )
{
    auto  w = new  TabArea();

    w->SetOnAreaBecomesEmptyCB( OnAreaBecomesEmptyCB_CloseTopLevel );
    w->SetOnTabDroppedOutCB( OnTabDroppedOutCB_RevertBack );
    
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->resize( 800, 30 );
    w->move(iTab->pos());
    w->setFocus();
    w->show();
    w->raise();
    w->DockHere(iTab);

    // Now this is optional if we want to activate the window once the tab has been docked
    // the set focus method just doesn't work
    // This does activate & raise the window but doesn't grab the focus
    w->raise();  // for MacOS
    w->activateWindow(); // for Windows
}


} // namespace  Rivet

