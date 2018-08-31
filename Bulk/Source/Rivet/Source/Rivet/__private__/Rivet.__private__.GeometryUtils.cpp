/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.__private__.GeometryUtils.cpp
* 9-9-2018 15:07 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#include "Rivet/__private__/Rivet.__private__.GeometryUtils.h"


#include <QWidget>


namespace  Rivet
{
namespace  __private__
{


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

