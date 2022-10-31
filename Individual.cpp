//
// Created by rafael on 22/02/2022.
//

#include "Individual.h"

//Constructor
Individual::Individual(bool initialzie, Configures *conf, Grammar *grammar) {

    this->conf=conf;
    this->grammar=grammar;
    this->costs= nullptr;
    this->times= nullptr;

    if(initialzie) {
        this->tree = new Tree(this->conf, this->grammar);
    }

}

//Destructor
Individual::~Individual() {
    if(this->costs == nullptr) {
        delete[] this->costs;
    }

    if(this->times == nullptr) {
        delete[] this->times;
    }
}

//Return an individual clone
Individual *Individual::clone() {
    Individual* clone = new Individual(false, this->conf, this->grammar);

    clone->fitness=this->fitness;
    clone->tree=this->tree->clone();

    return clone;
}

//Stores results obtained by the individual
void Individual::updateResults(float *costs, double *times, int n) {
    if(this->costs == nullptr){
        this->costs=new float[n];
    }

    if(this->times == nullptr){
        this->times=new double[n];
    }
    for(int i=0;i<n;i++){
        this->costs[i]=costs[i];
        this->times[i]=times[i];
    }
}

