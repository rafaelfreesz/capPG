//
// Created by rafael on 08/06/22.
//
#include <cmath>
#include <vector>
#include "Instance.h"

#ifndef CAPHH_ARENA_H
#define CAPHH_ARENA_H


class Arena {

public:

    Arena(Instance *instancia);
    ~Arena();

    void go(vector <string>* tokenizedIndividual);
    void interpretGrammar(vector <string>* linearIndividual);

    void calculateSolution();
    void calculateAbcissa();
    void calculateCost();
    void importSolution();
    void swapIndex(int i, int j);

    //operations//
    //pert//
    void swap(float iF, float jF);
    void repart(float fFactor);
    void shake (float sSizeRate);
    void rec(float rSizeRate);

    void insert(int iSource, int iDestino);
    //ref
    void bL1();
    void bL2();
    void bL3();
    void bL4();
    void blPc();

    int giveMeIndex(float value);
    void RVND();
    void bLPhase1(bool* improved);
    void bLPhase2(bool* improved);
    void bLPhase3(bool* improved);
    void bLPhase4(bool* improved);


    //Attributes
    Instance* instance;
    int* corridor;
    float* abcissa;
    int p;
    float cost;
    int n;

    void printLayout();
};


#endif //CAPHH_ARENA_H
