#include "Algorithm1.h"
#include "Helper.h"



void algorithm1(long T, const Arrangement_2& A){
    Halfedge_circulator e = getEdgesOfArrangement(A)[1];
    Point p = e -> target() -> point();
    Point pStart = p;
    std::set<Point> S;
    S.insert(p);
    Point repetitionPoint;
    int i, j;

    std::vector<Point> Gs;
    std::vector<Point> Cs;
    std::vector<std::vector<Point>> VPs;

    // for-loop over T
    for (i = 0; i < T; i++){
        auto [guard, e_new, p_new, isFinished] =  greedyStep(A, e, p);
        if (isFinished) { // isFinished is otherwise used to check if the start point is passed, this is a hack
//            std :: cout << "Trivially solved by one guard << std::endl";
            return;
        }
        Gs.emplace_back(guard);
        Cs.emplace_back(p);
        Cs.emplace_back(p_new);
        p = p_new;
        e = e_new;
        VPs.emplace_back(getVerticesOfArrangement(computeVisibilityArrangementGeneral(A, guard)));
//        std::cout << "GREEDY STEP " << i << " COMPLETED and ended at point: " << p << " and edge: " <<
//                  e->source()->point() << " -> " << e->target()->point() << " with guard at: " << guard << std::endl;
        // if p in S then do one final round until you p again
        if (S.contains(p)){
//            std::cout << "Found repetition with point p: " << p << " after i = " << i << " greedy steps" << std::endl;
            repetitionPoint = p;
            break;
        }
        S.insert(p);
    }



//    std::vector<Point> Gs;
//    std::vector<Point> Cs;
//    std::vector<std::vector<Point>> VPs;
    for (j = 0; j < A.number_of_vertices(); j++) {
        std::optional<Point> start = (j != 0 ? std::optional<Point>(repetitionPoint) : std::nullopt);
        auto [guard, e_new, p_new, isFinished] =  greedyStep(A, e, p, start); // skip first step where p == start

        Gs.emplace_back(guard);
        Cs.emplace_back(p);
        Cs.emplace_back(p_new);
        p = p_new;
        e = e_new;
        VPs.emplace_back(getVerticesOfArrangement(computeVisibilityArrangementGeneral(A, guard)));

//        std::cout << "GREEDY STEP " << j << " COMPLETED FOR SOLUTION and ended at point: " << p << " and edge: " <<
//                  e->source()->point() << " -> " << e->target()->point() << " with guard at: " << guard << std::endl;
        if (isFinished) {
//            std::cout << "Minimum number of guards: " << j + 1 << std::endl;
            break;
        }
    }
    // TODO: also add seed
    writeOutput("../Data/Local/results2.txt", i, j, A, Gs, Cs, VPs, pStart, true);
}


















