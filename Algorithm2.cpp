#include "Algorithm2.h"


std::tuple<Point, Halfedge_circulator, Point, bool> greedyInterval(const Arrangement_2& A, Halfedge_circulator e, Point p, const std::optional<Point>& start) {
    bool isFinished = false;

    Arrangement_2 F1 = computeVisibilityArrangementAtEdge(A, p, e);
    Polygon_set_2 F;
    F.insert(arrangement_to_polygon(F1));
    std::vector<Point> Fs;

    if (p == e->target()->point()){
        e++;
    }

    // #==================================================#
    // #  1) EXPAND THE GREEDY INTERVAL BY ENTIRE EDGES   #
    // #==================================================#

    int i = 0;
    for (; i < A.number_of_vertices() + 2; i++){
        p = e->target()->point();

        Polygon_2 VP = arrangement_to_polygon(computeVisibilityArrangementAtEdge(A, p, e));

        if (not Fs.empty()) {
            if (VP.has_on_unbounded_side(Fs[0])) {
                break;
            }
        } else {
            if (F.do_intersect(VP)) {
                F.intersection(VP);
            } else {
                // also check if F and VP intersect at their boundary at a single point (for general position input they cannot share a segment)
                auto FP = polygon_set_to_polygon(F);
                std::vector<Point> I = findAllIntersectionsBetweenEdgesOfPolygons(FP, VP);

                if (size(I) == 0) {
                    break;
                }  else {
                    Fs.push_back(I[0]);
                }
            }
        }

        // check if we passed the starting point and have found a solution
        isFinished = isFinished or passedStart(e, start, p);
        e++;
    }

    // If greedyInterval can traverse the entire polygon boundary then it is trivially solved by one guard
    if (i == A.number_of_vertices() + 2){
        return std::make_tuple(p, e, p, true);
        }

    // #===================================================#
    // #  2) EXPAND THE GREEDY INTERVAL ON THE FINAL EDGE  #
    // #===================================================#

    Point pFirst = e->source()->point();
    Point pBest = e->source()->point();

    Arrangement_2 FA;
    Point guard;

    // set FA to be the arrangement for the remaining feasible region
    if (not Fs.empty()){
        for (auto f: Fs){
            FA.insert_in_face_interior(f, FA.unbounded_face());
        }
        guard = Fs[0];
    } else {
        FA = polygon_set_to_arrangement(F);
        guard = FA.vertices_begin()->point();
    }

    // loop over vertices of FA and compute the visibility polygon of each vertex 'f' to find the point furthest on 'e'
    for (auto vit = FA.vertices_begin(); vit != FA.vertices_end(); ++vit) {
        Point f = vit->point();
        Arrangement_2 VP_f;
        VP_f = computeVisibilityArrangementGeneral(A, f);

        // loop over vertices of the visibility polygon
        for (auto vit2 = VP_f.vertices_begin(); vit2 != VP_f.vertices_end(); ++vit2) {
            Point q = vit2->point();
            if (pointIsOnEdgeButNotSource(q, e)) {

                // Check if vertex is further than pBest
                if (CGAL::squared_distance(pFirst, q) > CGAL::squared_distance(pFirst, pBest)) {
                    pBest = q;
                    guard = f;
                }
            }
        }
    }

    isFinished = isFinished or passedStart(e, start, pBest);

    if (pBest == pFirst){
        e--;
    }

    return std::make_tuple(guard, e, pBest, isFinished);
}

