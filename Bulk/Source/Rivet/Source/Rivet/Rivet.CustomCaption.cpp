/**
*
*   Rivet
*__________________
*
* @file     Rivet.CustomCaption.cpp
* @author   Clement Berthaud
* @brief    This file provides the definition for the RCustomCaption class.
*/
#include "Rivet/Rivet.CustomCaption.h"
#include <QtWinExtras> // for native event

namespace  Rivet {
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
RCustomCaption::~RCustomCaption()
{
}


RCustomCaption::RCustomCaption( QWidget* iParent )
    : tSuperClass( iParent )
{
}


//--------------------------------------------------------------------------------------
//----------------------------------------- Custom NC Handling implementation in caption
bool
RCustomCaption::HitEmptySpace( long iX, long iY )
{
    QPoint local = mapFromParent( QPoint( iX, iY ) );

    // Iterate over children and return false if hit children
    QList<QWidget*> list = this->findChildren<QWidget*>();
    for( QWidget *w : list) {
        if( w->parent() != this )
            continue;

        QRect g = w->geometry();
        if(g.contains( local ))
            return  false;
    }

    // No child was hit, return true.
    return  true;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
bool
RCustomCaption::nativeEvent( const  QByteArray& eventType, void* message, long* result )
{
    // Cast to WINAPI standards
    MSG*    wmsg    = reinterpret_cast< MSG* >( message );
    LPARAM  lparam  = wmsg->lParam;
    WPARAM  wparam  = wmsg->wParam;
    HWND    hwnd    = wmsg->hwnd;
    UINT    msg     = wmsg->message;

    switch( msg )
    {
        case WM_NCHITTEST:
        {
            *result = HTTRANSPARENT;
            return  true;
        }

        default:
        {
            // The case is not handled ? Treat it the normal way & return.
            return  tSuperClass::nativeEvent( eventType, message, result );
        }
    }
}


//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- Public Qt Slots
void
RCustomCaption::ProcessCloseClicked()
{
    emit  CloseClicked();
}


void
RCustomCaption::ProcessMaximizeClicked()
{
    emit  MaximizeClicked();
}


void
RCustomCaption::ProcessMinimizeClicked()
{
    emit  MinimizeClicked();
}


} // namespace  Rivet

