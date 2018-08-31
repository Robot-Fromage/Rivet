/*************************************************************************
*
*   Lavabo
*__________________
*
* Rivet.__private__.ForwardNC.h
* 3-10-2018 21:35 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE
*/

#pragma once

#include <windows.h>

static void DebugForward( char* iStr )
{
    auto dummy = 0;
}

#define  ForwardNC_Def virtual  bool  nativeEvent( const  QByteArray& eventType, void* message, long* result )  override;
#define  ForwardNC_Imp( iClass )                                                            \
    bool                                                                                    \
    iClass::nativeEvent( const  QByteArray&  eventType, void* message, long* result )       \
    {                                                                                       \
        MSG*    wmsg    = reinterpret_cast< MSG* >( message );                              \
        LPARAM  lparam  = wmsg->lParam;                                                     \
        WPARAM  wparam  = wmsg->wParam;                                                     \
        HWND    hwnd    = wmsg->hwnd;                                                       \
        UINT    msg     = wmsg->message;                                                    \
                                                                                            \
        switch( msg )                                                                       \
        {                                                                                   \
            case WM_NCHITTEST:                                                              \
            {                                                                               \
                DebugForward( #iClass );                                                    \
                *result = HTTRANSPARENT;                                                    \
                return  true;                                                               \
            }                                                                               \
                                                                                            \
            default:                                                                        \
            {                                                                               \
                return  QWidget::nativeEvent( eventType, message, result );                 \
            }                                                                               \
        }                                                                                   \
    }

