//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_INDIVIDUO_H
#define CAPHH_INDIVIDUO_H
#include "Arvore.h"
#include "Configuracoes.h"
#include "Gramatica.h"

using namespace std;
class Individuo {
public:
    Individuo(bool inicializa, Configuracoes *conf, Gramatica *grammar);

    virtual ~Individuo();

    Individuo* clone();

    void atualizarResultados(float *solucoes, double *tempos, int n);

    double fitness;
    Arvore* tree;
    Configuracoes* conf;
    Gramatica* gramatica;
    string nome;
    vector<string> terminaisLinearizados;
    float* solucoes;
    double* tempos;
};


#endif //CAPHH_INDIVIDUO_H
