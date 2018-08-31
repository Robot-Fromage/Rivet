/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.DockingCallbackLibrary.h
* 9-9-2018 12:10 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma once


namespace  Rivet
{

class Tab;
class TabArea;

void  OnAreaBecomesEmptyCB_CloseTopLevel( TabArea* iArea );
void  OnAreaBecomesEmptyCB_DoNothing( TabArea* iArea );
void  OnTabDroppedOutCB_RevertBack( Tab* iTab, TabArea* iSrc );
void  OnTabDroppedOutCB_Open( Tab* iTab, TabArea* iSrc );

} // namespace  Rivet

