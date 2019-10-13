/**
*
*   Rivet
*__________________
*
* @file     Rivet.__private__.GeometryUtils.h
* @author   Clement Berthaud
* @brief    This file provides the declarations for some Win32 utilities.
*/
#pragma once
#define NOMINMAX
#include <windows.h>

namespace  Rivet {
namespace  __private__ {
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

