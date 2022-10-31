//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_INDIVIDUO_H
#define CAPHH_INDIVIDUO_H
#include "Tree.h"
#include "Configures.h"
#include "Grammar.h"

using namespace std;
class Individuo {
public:
    Individuo(bool inicializa, Configures *conf, Grammar *grammar);

    virtual ~Individuo();

    Individuo* clone();

    void atualizarResultados(float *solucoes, double *tempos, int n);

    double fitness;
    Tree* tree;
    Configures* conf;
    Grammar* gramatica;
    string nome;
    vector<string> terminaisLinearizados;
    float* solucoes;
    double* tempos;
};


#endif //CAPHH_INDIVIDUO_H
