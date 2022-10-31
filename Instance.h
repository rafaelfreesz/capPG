//
// Created by rafael on 13/05/22.
//

#ifndef CAPHH_INSTANCE_H
#define CAPHH_INSTANCE_H
#include <iostream>
#include <fstream>
using namespace std;

class Instance {

public:
    Instance ();
    ~Instance();

    //Instance attributes
    string name;
    int n;
    float literatureCost;
    int* lengths;
    int** demands;

    //Base solution attributes
    int partition;
    float cost;
    int* corridor;
    int* abcissas;


    void buildLengths();
    void buildDemands();
    void buildInitialSolution();

    void printLengths();
    void printDemands();
    void printLayout();

    void swapIndex(int i, int j);
    void calculateSolution();

};


#endif //CAPHH_INSTANCE_H
