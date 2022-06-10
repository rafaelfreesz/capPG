//
// Created by rafael on 22/02/2022.
//

#include "Individuo.h"

Individuo::Individuo(bool inicializa, Configuracoes *conf, Gramatica *grammar) {

    this->conf=conf;
    this->gramatica=grammar;
    this->solucoes= nullptr;
    this->tempos= nullptr;

    if(inicializa) {
        this->tree = new Arvore(this->conf, this->gramatica);
    }

}


Individuo::~Individuo() {
    if(this->solucoes== nullptr) {
        delete[] this->solucoes;
    }

    if(this->tempos== nullptr) {
        delete[] this->tempos;
    }
}

Individuo *Individuo::clone() {
    Individuo* clone = new Individuo(false, this->conf, this->gramatica);

    clone->fitness=this->fitness;
    clone->tree=this->tree->clone();

    return clone;
}

void Individuo::atualizarResultados(float *solucoes, double *tempos, int n) {
    if(this->solucoes== nullptr){
        this->solucoes=new float[n];
    }

    if(this->tempos== nullptr){
        this->tempos=new double[n];
    }
    for(int i=0;i<n;i++){
        this->solucoes[i]=solucoes[i];
        this->tempos[i]=tempos[i];
    }
}

