/**
*
*   Rivet
*__________________
*
* @file     Rivet.DockingCallbackLibrary.h
* @author   Clement Berthaud
* @brief    This file provides the declaration for the Docking callbacks functions.
*/
#pragma once

namespace  Rivet {

class RTab;
class RTabArea;

void  OnAreaBecomesEmptyCB_CloseTopLevel( RTabArea* iArea );
void  OnAreaBecomesEmptyCB_DoNothing( RTabArea* iArea );
void  OnTabDroppedOutCB_RevertBack( RTab* iTab, RTabArea* iSrc );
void  OnTabDroppedOutCB_Open( RTab* iTab, RTabArea* iSrc );

} // namespace  Rivet

