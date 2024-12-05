#include "Algorithm1.h"
#include "Helper.h"



void algorithm1(long T, const Arrangement_2& A){
    std::cout << "Algorithm 1 running" << std::endl;

    // Choose arbitrary starting point x_0
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
        if (isFinished) {
            return;
        }
        Gs.emplace_back(guard);
        Cs.emplace_back(p);
        Cs.emplace_back(p_new);
        p = p_new;
        e = e_new;
        VPs.emplace_back(getVerticesOfArrangement(computeVisibilityArrangementGeneral(A, guard)));

        // if p in S then do one final round to find an optimal solution
        if (S.contains(p)){
            repetitionPoint = p;
            break;
        }
        S.insert(p);
    }


    for (j = 0; j < A.number_of_vertices(); j++) {
        std::optional<Point> start = (j != 0 ? std::optional<Point>(repetitionPoint) : std::nullopt);
        auto [guard, e_new, p_new, isFinished] =  greedyStep(A, e, p, start);

        Gs.emplace_back(guard);
        Cs.emplace_back(p);
        Cs.emplace_back(p_new);
        p = p_new;
        e = e_new;
        VPs.emplace_back(getVerticesOfArrangement(computeVisibilityArrangementGeneral(A, guard)));

        if (isFinished) {
            break;
        }
    }

    writeOutput("../outputs.txt", i, j, A, Gs, Cs, VPs, pStart, true); // < --- UNCOMMENT THIS LINE TO WRITE OUTPUT TO FILE
}


















