#include "Helper.h"
#include <CGAL/Polygon_2.h>



Arrangement_2 computeVisibilityArrangementAtEdge(const Arrangement_2 & A, const Point& p, Halfedge_const_handle e) {
    Arrangement_2 VA;
    VA_type VP_calculator(A);
    VP_calculator.compute_visibility(p, e, VA);
    return VA;
}

Arrangement_2 computeVisibilityArrangementAtFace(const Arrangement_2 & A, const Point& p) {
    Face_const_handle f;
    if (A.edges_begin()->face()->is_unbounded()){
        f = A.edges_begin()->twin()->face();
    }
    else {
        f = A.edges_begin()->face();
    }

    Arrangement_2 VA;
    VP_type_non_regular_face VP_calculator(A);
    VP_calculator.compute_visibility(p, f, VA);

    return VA;
}

Arrangement_2 computeVisibilityArrangementGeneral(const Arrangement_2& A, const Point& p) {
    Arrangement_2 VP;

    for (auto eA: getEdgesOfArrangement(A)) {
        if (pointIsOnEdgeButNotSource(p, eA)) {
            VP = computeVisibilityArrangementAtEdge(A, p, eA);
            break;
        }
    }

    if (VP.is_empty()) {
        VP = computeVisibilityArrangementAtFace(A, p);
    }
    return VP;
}


std::vector<Point> findAllIntersectionsBetweenEdgesOfPolygons(const Polygon_2& P, const Polygon_2& Q){
    std::vector<Point> I;
    for (auto eP: P.edges()) {
        for (auto eQ: Q.edges()) {
            auto intersection = CGAL::intersection(eP, eQ);
            if (intersection) {
                const Point *pI = std::get_if<Point>(&*intersection);
                I.push_back(*pI);
            }
        }
    }
    return I;
}

bool passedStart(const Halfedge_circulator e, const std::optional<Point>& start, const Point p) {
    if (start.has_value() and pointIsOnEdgeButNotSource(start.value(), e)){
        // check if 'p' is closer than 'start' to 'e''s target
        return CGAL::squared_distance(p, e->target()->point()) <= CGAL::squared_distance(start.value(), e->target()->point());
    }
    return false;
}

bool isInGeneralPosition(const Arrangement_2 & A) {
    std::vector<Point> Ps = getVerticesOfArrangement(A);
    int n = Ps.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            for (int k = j + 1; k < n; ++k) {
                if (CGAL::collinear(Ps[i], Ps[j], Ps[k])) {
                    return false;
                }
            }
        }
    }
    return true;
}

void writeOutput(const std::string& filename, int i, int j, const Arrangement_2& A, const std::vector<Point>& Gs, const std::vector<Point>& Cs, const std::vector<std::vector<Point>>& VPs, Point pStart, bool verbose) {
    std::ostringstream output;
    output << "Found solution with " << j + 1 << " guards in " << i + j + 2 << " greedy steps from pStart: " << pStart<< " (input in general position: " << isInGeneralPosition(A) << ")" << std::endl;
    output << "Vertices of the polygon: ";
    for (const auto& v : getVerticesOfArrangement(A)) {
        output << v << " ";
    }

    for (int k = 0; k < Gs.size(); k++) {
        output << std::endl << "Guard " << k << " at: " << Gs[k] << " with chain interval: "
               << Cs[2 * k] << " " << Cs[2 * k + 1]
               << " and visibility polygon vertices: ";
        for (const auto& v : VPs[k]) {
            output << v << " ";
        }
    }

    output << std::endl << std::endl;

    if (verbose) {
        std::cout << "Found solution with " << j + 1 << " guards in " << i + j + 2
                  << " greedy steps (input in general position: " << isInGeneralPosition(A) << ")" << std::endl
                  << std::endl;
    }

    std::ofstream file(filename, std::ios::app);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }
    file << output.str();
    file.close();
}



std::vector<Halfedge_circulator> getEdgesOfArrangement(const Arrangement_2& P) {
    auto face = P.faces_begin();
    if (face->is_unbounded()) {
        face++;
    }

    std::vector<Halfedge_circulator> E;
    auto curr = face->outer_ccb();

    do {
        E.push_back(curr);
        curr++;
    } while (curr != face->outer_ccb());

    return E;
}

std::vector<Point> getVerticesOfArrangement(const Arrangement_2& P) {
    std::vector<Point> V;
    for (auto e : getEdgesOfArrangement(P)) {
        V.emplace_back(e->source()->point());
    }
    return V;
}


Polygon_2 arrangement_to_polygon(const Arrangement_2& A) {
    Polygon_2 polygon;

    for (auto e : getEdgesOfArrangement(A)) {
        polygon.push_back(e->source()->point());
    }

    return polygon;
}

Arrangement_2 polygon_to_arrangement(const Polygon_2& polygon) {
    Arrangement_2 A;

    std::vector<Segment> segments;
    for (auto it = polygon.edges_begin(); it != polygon.edges_end(); ++it) {
        segments.push_back(*it);
    }

    CGAL::insert_non_intersecting_curves(A, segments.begin(), segments.end());

    return A;
}


Arrangement_2 polygon_set_to_arrangement(const Polygon_set_2& polygon_set) {
    std::list<Polygon_with_holes_2> polygons;
    polygon_set.polygons_with_holes(std::back_inserter(polygons));
    const Polygon_with_holes_2& pwh = polygons.front();
    const Polygon_2& outer_boundary = pwh.outer_boundary();
    Arrangement_2 A;

    std::vector<Segment> segments;
    for (auto it = outer_boundary.edges_begin(); it != outer_boundary.edges_end(); ++it) {
        segments.push_back(*it);
    }
    CGAL::insert_non_intersecting_curves(A, segments.begin(), segments.end());

    return A;
}

Polygon_2 polygon_set_to_polygon(const Polygon_set_2& polygon_set) {
    std::list<Polygon_with_holes_2> polygons;
    polygon_set.polygons_with_holes(std::back_inserter(polygons));
    const Polygon_with_holes_2& pwh = polygons.front();
    const Polygon_2& outer_boundary = pwh.outer_boundary();

    return outer_boundary;

}

void printArrangementEdges(const Arrangement_2& A, const std::string& name) {
    std::cout << name << ": ";
    std::vector<Halfedge_circulator > E = getEdgesOfArrangement(A);

    for (auto e : E) {
        std::cout << "(" << e->source()->point() << " -> " << e->target()->point() << ") ";
    }
    std::cout << std::endl;
}

bool pointIsOnEdgeButNotSource(const Point& p, Halfedge_circulator e) {
    const Point& source = e->source()->point();
    const Point& target = e->target()->point();

    // Allow p to the target of the edge but not the source
    if (p == source) {
        return false;
    }

    if (p == target) {
        return true;
    }

    std::ostringstream oss;
    // Check if p is collinear with the edge
    if (!CGAL::collinear(source, target, p)) {
        return false;
    }

    // Check if point p is within the segment bounds
    if (!(CGAL::compare(CGAL::min(source.x(), target.x()), p.x()) != CGAL::LARGER &&
          CGAL::compare(CGAL::max(source.x(), target.x()), p.x()) != CGAL::SMALLER &&
          CGAL::compare(CGAL::min(source.y(), target.y()), p.y()) != CGAL::LARGER &&
          CGAL::compare(CGAL::max(source.y(), target.y()), p.y()) != CGAL::SMALLER)) {
        return false;
    }
    return true;
}

void drawArrangements(const Arrangement_2& A1, const Arrangement_2& A2) {
    Arrangement_2 A_merged;
    for (auto edge = A1.edges_begin(); edge != A1.edges_end(); ++edge) {
        A_merged.insert_in_face_interior(Segment(edge->source()->point(), edge->target()->point()), A_merged.unbounded_face());
    }

    for (auto edge = A2.edges_begin(); edge != A2.edges_end(); ++edge) {
        A_merged.insert_in_face_interior(Segment(edge->source()->point(), edge->target()->point()), A_merged.unbounded_face());
    }
    CGAL::draw(A_merged);
}
