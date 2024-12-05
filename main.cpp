#include <iostream>
#include <cstdlib>

#include <future>
#include <vector>
#include <chrono>

#include "Algorithm1.h"
#include "Helper.h"

// for random polygon
#include <CGAL/random_polygon_2.h>
#include <CGAL/Random.h>
#include <CGAL/point_generators_2.h>
typedef CGAL::Creator_uniform_2<double, Point> PointCreator;
typedef CGAL::Random_points_in_square_2<Point, PointCreator> Point_generator;

Polygon_2 generate_random_polygon(int n, std::optional<int> seed = std::nullopt) {
    Polygon_2 P;
    std::vector<Point> points;
    CGAL::Random rand;
    if (seed.has_value()) {
        rand = CGAL::Random(seed.value());
    }

    CGAL::copy_n_unique(Point_generator(1000, rand), n, std::back_inserter(points));
    CGAL::random_polygon_2(points.size(), std::back_inserter(P), points.begin());

    return P;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Not enough parameters provided." << std::endl;
        return 1;
    }

    // Parse input parameters
    int n = std::atoi(argv[1]);

    //    Polygon needing 3 guards: https://www.desmos.com/geometry/6iocdgnncv
    //    std::vector<Point> points = {Point(0,0), Point(5,-1), Point(4,0), Point(8,-2), Point(8,2), Point(4, 1), Point(6, 4),
    //                                     Point(10, 2), Point(8, 8), Point(4, 8), Point(6, 10), Point(0, 10), Point(0, 6), Point(8, 6)};

    //    Polygon needing 2 guards: https://www.desmos.com/geometry/ekruamhxwc
    //    std::vector<Point> points = {Point(0,0), Point(3, 2), Point(4, 0), Point(4 ,5), Point(1, 2), Point(0, 4)};
    //
    //    std::vector<Segment> segments;
    //    for (int i = 0; i < points.size(); i++) {
    //        segments.emplace_back(points[i], points[(i + 1) % points.size()]);
    //    }

    Arrangement_2 P = polygon_to_arrangement(generate_random_polygon(n, 0));  // change seed to get a different random polygons

    algorithm1(100 * n, P);


    return 1;
}


