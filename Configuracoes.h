//
// Created by rafael on 21/02/2022.
//

#ifndef CAPHH_CONFIGURACOES_H
#define CAPHH_CONFIGURACOES_H


class Configuracoes {

public:

    Configuracoes(int profMaxima, int geracoes, int tamPop, double taxaCrossover,
    double taxaMutacao,double elitismo, int numParentais);


    int profMaxima;
    int tamPop;
    int geracoes;
    int numParentais;
    double taxaMutacao;
    double taxaCrossover;
    double elitismo;
    int sementeGeracao;
};


#endif //CAPHH_CONFIGURACOES_H
