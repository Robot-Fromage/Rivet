/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.__private__.ASyncJS_PPFramework.h
* 9-9-2018 10:44 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma  once


#include  <QEventLoop>
#include  <QVariant>


#define  Rivet_ASyncJS_RFlag_Name                     ASyncJS_ReadyFlag
#define  Rivet_ASyncJS_VRet_Name                      ASyncJS_VRet
#define  Rivet_ASyncJS_SetReadySlot_Name              ASyncJS_SetReady
#define  Rivet_ASyncJS_UnlockFct_Name                 ASyncJS_Unlock_fct
#define  Rivet_ASyncJS_UnlockSignal_Name              ASyncJS_Unlock_sig

#define  Rivet_ASyncJS_Decl_RFlag                     const  bool  Rivet_ASyncJS_RFlag_Name = false;
#define  Rivet_ASyncJS_Decl_VRet                      QVariant  Rivet_ASyncJS_VRet_Name;
#define  Rivet_ASyncJS_Decl_SetReadySlot              void  Rivet_ASyncJS_SetReadySlot_Name( bool );
#define  Rivet_ASyncJS_Decl_UnlockFct                 void  Rivet_ASyncJS_UnlockFct_Name();
#define  Rivet_ASyncJS_Decl_UnlockSignal              void  Rivet_ASyncJS_UnlockSignal_Name();

#define  Rivet_ASyncJS_Set_RFlag                      bool* t = const_cast< bool* >( &Rivet_ASyncJS_RFlag_Name ); *t = true;

#define  Rivet_ASyncJS_Impl_SetReadySlot( iClass )    void  iClass::Rivet_ASyncJS_SetReadySlot_Name( bool ) { Rivet_ASyncJS_Set_RFlag; };
#define  Rivet_ASyncJS_Impl_UnlockFct( iClass )       void  iClass::Rivet_ASyncJS_UnlockFct_Name() { emit  Rivet_ASyncJS_UnlockSignal_Name(); }

#define  Rivet_ASyncJS_IsReady                        Rivet_ASyncJS_RFlag_Name

#define  Rivet_ASyncJS_Decl                           private: Rivet_ASyncJS_Decl_UnlockFct                                                 \
                                                        private: Rivet_ASyncJS_Decl_RFlag Rivet_ASyncJS_Decl_VRet                           \
                                                        Q_SLOT Rivet_ASyncJS_Decl_SetReadySlot                                              \
                                                        Q_SIGNAL Rivet_ASyncJS_Decl_UnlockSignal

#define  Rivet_ASyncJS_Impl( iClass )                 Rivet_ASyncJS_Impl_SetReadySlot( iClass ) Rivet_ASyncJS_Impl_UnlockFct( iClass )

#define  Rivet_ASyncJS_Run( iContext, iCmd, iResult )                                                                                       \
    QEventLoop loop;                                                                                                                        \
    QObject::connect( this, SIGNAL( ASyncJS_Unlock_sig() ), &loop, SLOT( quit() ) );                                                        \
    iContext->runJavaScript( iCmd, [this]( const  QVariant&  v ) { Rivet_ASyncJS_VRet_Name =  v; Rivet_ASyncJS_UnlockFct_Name(); } );       \
    loop.exec();                                                                                                                            \
    iResult = Rivet_ASyncJS_VRet_Name;

#define  Rivet_ASyncJS_ReadyCond( iObj, iSignal )                                                                                           \
    QObject::connect( iObj, SIGNAL( iSignal( bool ) ), this, SLOT( ASyncJS_SetReady_slot( bool ) ) );

