/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.CustomCaption.cpp
*
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include "Rivet/Rivet.CustomCaption.h"


#include <QtWinExtras>


namespace  Rivet
{


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


CustomCaption::~CustomCaption()
{
}

CustomCaption::CustomCaption( QWidget* parent ) :
    tSuperClass( parent )
{
}


//--------------------------------------------------------------------------------------
//----------------------------------------- Custom NC Handling implementation in caption


bool
CustomCaption::HitEmptySpace( long iX, long iY )
{
    QPoint local = mapFromParent( QPoint( iX, iY ) );

    QList<QWidget*> list = this->findChildren<QWidget*>();

    for( QWidget *w : list) {
        if( w->parent() != this )
            continue;

        QRect g = w->geometry();
        if(g.contains( local ))
            return  false;
    }

    return  true;
}


//--------------------------------------------------------------------------------------
//----------------------------------------- Protected Qt / WinAPI native events override


bool
CustomCaption::nativeEvent( const  QByteArray& eventType, void* message, long* result )
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
//--------------------------------------------------------------------- Private Qt Slots


void
CustomCaption::ProcessCloseClicked()
{
    emit  CloseClicked();
}

void
CustomCaption::ProcessMaximizeClicked()
{
    emit  MaximizeClicked();
}


void
CustomCaption::ProcessMinimizeClicked()
{
    emit  MinimizeClicked();
}


} // namespace  Rivet

