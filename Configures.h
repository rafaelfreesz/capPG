//
// Created by rafael on 21/02/2022.
//

#ifndef CAPHH_CONFIGURES_H
#define CAPHH_CONFIGURES_H

// This class contains the parameters to be used by the genetic programming algorithm
class Configures {

public:

    Configures(int maxDeep, int generations, int popSize, double crossoverRate,
               double mutationRate, double elitism, int numParents){
        this->maxDeep=maxDeep;
        this->generations=generations;
        this->popSize=popSize;
        this->crossoverRate=crossoverRate;
        this->mutationRate=mutationRate;
        this->elitism=elitism;
        this->numParents=numParents;
        this->seed=0;
    }


    int maxDeep;
    int popSize;
    int generations;
    int numParents;
    double mutationRate;
    double crossoverRate;
    double elitism;
    int seed;
};


#endif //CAPHH_CONFIGURES_H
