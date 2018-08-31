/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.__private__.WinExtras.h
* 9-9-2018 12:10 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma once


// Fix conflict beetween Win32 & Qt Macros
#ifndef NOMINMAX
#define NOMINMAX
#endif // ! NOMINMAX


#include <windows.h>


namespace  Rivet
{
namespace  __private__
{


// Utility Enum for Windows Versions
enum  eWindowsVersion
{
    kBadValue = 0,
    kWindowsXP,
    kWindowsXPSP1,
    kWindowsXPSP2,
    kWindowsXPSP3,
    kWindowsVista,
    kWindowsVistaSP1,
    kWindowsVistaSP2,
    kWindows7,
    kWindows7SP1,
    kWindows8,
    kWindows8Point1,
    kWindows10
};


// Public utility Functions related to the Windows API
int                 GetZOrder( HWND iTarget );
eWindowsVersion     GetWindowsVersion();
bool                IsWindowsVersion( eWindowsVersion iWindowsVersion );
bool                EnableGlassForWindow8OrGreater( HWND iWindow );
bool                DisableGlassForWindow8OrGreater( HWND iWindow );
static HMODULE      _load_sys_library( const WCHAR* name);


} // namespace  __private__
} // namespace  Rivet

