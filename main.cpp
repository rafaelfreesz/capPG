#include <iostream>
#include "Configures.h"
#include "Grammar.h"
#include "Parser.h"
#include "Search.h"
using namespace std;

Configures *
buildGpConfig(int maxDeep, int generation, int popSize, double crossoverRate, double mutationRate, double elitism,
              int numIndividuosSelection);


int main(int argc, char** argv) {
    srand(clock());
    Configures* conf = buildGpConfig(10, 500, 100, 0.9, 0.1, 0.05, 2);

    Grammar* grammar=new Grammar("grammar.dat",conf);

    Parser* parser = new Parser();

    Search* search = new Search(parser, NULL, conf, grammar);

    search->evolve();

    delete conf;
    delete parser;
}

//Define as configurações da PG
Configures * buildGpConfig(int maxDeep, int generation, int popSize, double crossoverRate, double mutationRate, double elitism,
              int numIndividuosSelection) {
    Configures* conf = new Configures();

    conf->maxDeep=maxDeep;
    conf->generations=generation;
    conf->popSize=popSize;
    conf->crossoverRate=crossoverRate;
    conf->mutationRate=mutationRate;
    conf->evaluations=0;
    conf->elitism=elitism;
    conf->numIndividuosSelection=numIndividuosSelection;

    return conf;
}