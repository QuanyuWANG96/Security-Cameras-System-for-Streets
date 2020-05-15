//
// Created by quany on 11/20/2019.
//

#ifndef ECE650_PRJ_ECE650_H
#define ECE650_PRJ_ECE650_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include "minisat/mtl/IntTypes.h"
#include "minisat/mtl/Vec.h"
#include <memory>

#include <sys/time.h>
#include "pthread.h"


using namespace std;


typedef class Graph{
public:
    std::vector<int> edge;
    std::vector<std::vector <int >> adj;
    int maxVertex;

};


typedef class RESULT{
public:
    Graph *g;
    std::unique_ptr <Minisat::Solver>  solver;
    std::vector<int> vertexCover;
    std::string methodType;
    long int runTime;
    Minisat::lbool SAT(int kValue, vector<int> &result);
    std::vector<int> getResultCnfSat();
};


#endif //ECE650_PRJ_ECE650_H
