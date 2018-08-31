/*************************************************************************
*
*   Rivet
*__________________
*
* Rivet.__private__.GeometryUtils.h
* 9-9-2018 11:59 GMT+1
* Clement Berthaud - Layl
* Please refer to LICENSE.TXT
*/

#pragma once


class  QWidget;

#include <QRect>
#include <QSize>
#include <QPoint>


namespace  Rivet
{
namespace  __private__
{


QRect   MapRectToGlobal( QWidget* iFrom, const  QRect&  iRect );
QRect   MapRectFromGlobal( QWidget* iTo, const  QRect&  iRect );
QPoint  PointFromSize( const  QSize&  iFrom );
QSize   SizeFromPoint( const  QPoint&  iFrom );


} // namespace  __private__
} // namespace  Rivet

