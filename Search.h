//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_SEARCH_H
#define CAPHH_SEARCH_H
#include <iostream>
#include <vector>
#include "Individual.h"
#include "Grammar.h"
#include "Configures.h"
#include "Output.h"
#include "Instance.h"
#include "Arena.h"
#include <ctime>

using namespace std;
class Search {
public:
    Search(Configures *conf, int seed);
    ~Search();

    void evolve();
    void runAllInstances();
    void buildInitialPopulation();
    void evaluatePopulation(int initialIndex, int finalIndex, int generation);
    void evaluateIndividual(int index);
    static bool sortPopulationFitness(Individual* a, Individual* b);
    void randParents(Individual **parents, int ind);
    void operate();
    void replace();

    //-----Crossover Operators-----//
    void crossover(Individual** selected);
    void treeCrossover(Tree* a, Tree* b);

    //-----Mutation Operators-----//
    void mutate(Individual** selected);
    void auxMutate(Tree* t);

    //-----Pre-running-----//
    void buildInstances();
    void printIndividual(int index);
    void printIndividualResults(int index);
    void generateInitialSolutions();

    void tokenize(string str, vector<string> &token_v, string DELIMITER);

    Individual** population;
    Configures* conf;
    Grammar* grammar;
    string command;
    vector<Instance> instances;
    int seed;
};


#endif //CAPHH_SEARCH_H
