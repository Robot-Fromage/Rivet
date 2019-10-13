/**
*
*   Rivet
*__________________
*
* @file     Rivet.__private__.GeometryUtils.cpp
* @author   Clement Berthaud
* @brief    This file provides the definitions for some QT Geometry utilities.
*/
#include "Rivet/__private__/Rivet.__private__.GeometryUtils.h"
#include <QWidget>

namespace  Rivet {
namespace  __private__ {
QRect
MapRectToGlobal( QWidget* iFrom, const  QRect&  iRect )
{
    if( !iFrom )
        return  iRect;

    return  QRect(  iFrom->mapToGlobal( iRect.topLeft() ),
                    iFrom->mapToGlobal( iRect.bottomRight() ) );
}


QRect
MapRectFromGlobal( QWidget* iTo, const  QRect&  iRect )
{
    if( !iTo )
        return  iRect;
    return  QRect(  iTo->mapFromGlobal( iRect.topLeft() ),
                    iTo->mapFromGlobal( iRect.bottomRight() ) );
}


QPoint
PointFromSize( const  QSize&  iFrom )
{
    return  QPoint( iFrom.width(), iFrom.height() );
}


QSize
SizeFromPoint( const  QPoint&  iFrom )
{
    return  QSize( iFrom.x(), iFrom.y() );
}


} // namespace  __private__
} // namespace  Rivet

