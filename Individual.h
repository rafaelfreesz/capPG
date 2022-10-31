//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_INDIVIDUAL_H
#define CAPHH_INDIVIDUAL_H
#include "Tree.h"
#include "Configures.h"
#include "Grammar.h"

using namespace std;
class Individual {
public:
    Individual(bool initialzie, Configures *conf, Grammar *grammar);

    virtual ~Individual();

    Individual* clone();

    void updateResults(float *costs, double *times, int n);

    double fitness;
    Tree* tree;
    Configures* conf;
    Grammar* grammar;
    string name;
    vector<string> linearizedTerminals;
    float* costs;
    double* times;
};


#endif //CAPHH_INDIVIDUAL_H
