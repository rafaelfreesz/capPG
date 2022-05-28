//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_SEARCH_H
#define CAPHH_SEARCH_H
#include <iostream>
#include <vector>
#include "Parser.h"
#include "Subject.h"
#include "Grammar.h"
#include "Configures.h"
#include "Saida.h"
#include "Instances.h"
#include <ctime>

using namespace std;
class Search {
public:
    Search(Parser *parser, int *populationReplacement, Configures *conf,Grammar* grammar);
    ~Search();

    void evolve();
    void createInicialPopulation();
    void evaluatePopulation(int initialIndex, int finalIndex);
    void execute() const;
    void evaluateIndividual(int index);
    static bool sortPopulationFitness(Subject* a, Subject* b);
    void randPais(Subject **pais, int ind);
    void operate();
    void replace();
    //-----Elementos de Crossover-----//
    void crossover(Subject** selecionados);
    void treeCrossover(Tree* a, Tree* b);
    //-----Elementos de Mutação-----//
    void mutate(Subject** selecionados);
    void auxMutate(Tree* t);

    void exportGeneration(int gen);

    void tokenize(string str, vector<string> &token_v, string DELIMITER);

    Parser* parser;
    int* populationReplacement;
    stringstream saGpAgent;
    stringstream sbGpAgent;
    string aGpAgent;
    string bGpAgent;
    Subject** pop;
    Configures* conf;
    Grammar* grammar;
    Instances* instances;
    Saida* logFile;

};


#endif //CAPHH_SEARCH_H
