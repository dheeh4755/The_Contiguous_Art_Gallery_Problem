#ifndef CONTIGUOUSARTGALLERY_ALGORITHM2_H
#define CONTIGUOUSARTGALLERY_ALGORITHM2_H

#include "Helper.h"

std::tuple<Point, Halfedge_circulator, Point, bool> greedyStep(const Arrangement_2&, Halfedge_circulator e, Point p, const std::optional<Point>& start = std::nullopt);

Arrangement_2 computeVisibilityArrangementGeneral(const Arrangement_2& A, const Point& p);

#endif
