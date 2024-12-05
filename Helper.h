#ifndef CONTIGUOUSARTGALLERY_HELPER_H
#define CONTIGUOUSARTGALLERY_HELPER_H

#include <CGAL/draw_arrangement_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Triangular_expansion_visibility_2.h>
#include <CGAL/Simple_polygon_visibility_2.h>
#include <CGAL/Boolean_set_operations_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel                   Kernel;
typedef Kernel::Point_2                                                     Point;
typedef Kernel::Segment_2                                                   Segment;
typedef CGAL::Arr_segment_traits_2<Kernel>                                  Traits;
typedef CGAL::Arrangement_2<Traits>                                         Arrangement_2;
typedef CGAL::Polygon_2<Kernel>                                             Polygon_2;
typedef CGAL::Polygon_set_2<Kernel>                                         Polygon_set_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                                  Polygon_with_holes_2;
typedef Arrangement_2::Ccb_halfedge_const_circulator                        Halfedge_circulator;
typedef Arrangement_2::Halfedge_const_handle                                Halfedge_const_handle;
typedef CGAL::Triangular_expansion_visibility_2<Arrangement_2>              VA_type;
typedef CGAL::Simple_polygon_visibility_2<Arrangement_2, CGAL::Tag_false>   VP_type_non_regular_face;
typedef Arrangement_2::Face_const_handle                                    Face_const_handle;


Arrangement_2 computeVisibilityArrangementAtEdge(const Arrangement_2 & A, const Point& p, Halfedge_const_handle e);

Arrangement_2 computeVisibilityArrangementAtFace(const Arrangement_2 & A, const Point& p);

Arrangement_2 computeVisibilityArrangementGeneral(const Arrangement_2& A, const Point& p);

std::vector<Point> findAllIntersectionsBetweenEdgesOfPolygons(const Polygon_2& P, const Polygon_2& Q);

bool passedStart(const Halfedge_circulator e, const std::optional<Point>& start, const Point p);

bool isInGeneralPosition(const Arrangement_2 & A);

void writeOutput(const std::string& filename, int i, int j, const Arrangement_2& A, const std::vector<Point>& Gs, const std::vector<Point>& Cs, const std::vector<std::vector<Point>>& VPs, Point pStart, bool verbose = false);

void printArrangementEdges(const Arrangement_2& P, const std::string& name);

std::vector<Halfedge_circulator> getEdgesOfArrangement(const Arrangement_2& P);

std::vector<Point> getVerticesOfArrangement(const Arrangement_2& P);

void drawArrangements(const Arrangement_2& A1, const Arrangement_2& A2);

void validatePointOnEdge(const Point& p, Halfedge_circulator e);

bool pointIsOnEdgeButNotSource(const Point& p, Halfedge_circulator e);

Polygon_2 arrangement_to_polygon(const Arrangement_2& A);

Arrangement_2 polygon_to_arrangement(const Polygon_2& polygon);

Arrangement_2 polygon_set_to_arrangement(const Polygon_set_2& polygon);

Polygon_2 polygon_set_to_polygon(const Polygon_set_2& polygon_set);

#endif
