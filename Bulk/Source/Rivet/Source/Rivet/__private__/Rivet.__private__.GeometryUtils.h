/**
*
*   Rivet
*__________________
*
* @file     Rivet.__private__.GeometryUtils.h
* @author   Clement Berthaud
* @brief    This file provides the declarations for some QT Geometry utilities.
*/
#pragma once
#include <QRect>
#include <QSize>
#include <QPoint>

class  QWidget;

namespace  Rivet {
namespace  __private__ {
QRect   MapRectToGlobal( QWidget* iFrom, const  QRect&  iRect );
QRect   MapRectFromGlobal( QWidget* iTo, const  QRect&  iRect );
QPoint  PointFromSize( const  QSize&  iFrom );
QSize   SizeFromPoint( const  QPoint&  iFrom );
} // namespace  __private__
} // namespace  Rivet

