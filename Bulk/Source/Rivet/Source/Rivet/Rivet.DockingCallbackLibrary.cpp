/**
*
*   Rivet
*__________________
*
* @file     Rivet.DockingCallbackLibrary.h
* @author   Clement Berthaud
* @brief    This file provides the definitions for the Docking callbacks functions.
*/
#include "Rivet/Rivet.DockingCallbackLibrary.h"
#include "Rivet/Rivet.TabArea.h"

namespace  Rivet {


void
OnAreaBecomesEmptyCB_CloseTopLevel( RTabArea* iArea )
{
    auto tw = iArea->topLevelWidget();
    tw->close();
    tw->deleteLater();
}


void
OnAreaBecomesEmptyCB_DoNothing( RTabArea* iArea )
{
}


void
OnTabDroppedOutCB_RevertBack( RTab* iTab, RTabArea* iSrc )
{
    iSrc->DockHere( iTab );
}


void
OnTabDroppedOutCB_Open( RTab* iTab, RTabArea* iSrc )
{
    auto  w = new  RTabArea();

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

