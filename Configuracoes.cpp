//
// Created by rafael on 21/02/2022.
//

#include "Configuracoes.h"

Configuracoes::Configuracoes(int profMaxima, int geracoes, int tamPop, double taxaCrossover,
                             double taxaMutacao,double elitismo, int numParentais){
    this->profMaxima=profMaxima;
    this->geracoes=geracoes;
    this->tamPop=tamPop;
    this->taxaCrossover=taxaCrossover;
    this->taxaMutacao=taxaMutacao;
    this->elitismo=elitismo;
    this->numParentais=numParentais;
    this->sementeGeracao=0;
    //this->evaluations=0;

}
