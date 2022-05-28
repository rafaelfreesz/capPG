//
// Created by rafael on 13/02/2022.
//

#include "Saida.h"

Saida::Saida(string arquivo) {
    this->arquivo=arquivo;
    abrirArquivo();
}
Saida::~Saida() {
    fecharArquivo();
}

void Saida::abrirArquivo() {
    this->saida.open(this->arquivo);
}
void Saida::fecharArquivo() {
    if (this->saida.is_open()){
        this->saida.close();
    }
}

void Saida::imprimirResultado(Subject **pop, int popSize, int generation, int seed) {
    this->saida<<to_string(generation)<<" | "<<to_string(seed)<<" | ";
    for(int i=0;i<popSize;i++){
        this->saida<<pop[i]->fitness<<" ("<< pop[i]->subjectName<<") - ";
    }
    this->saida<<endl;
}

void Saida::escreverLinha(string linha) {
    this->saida<<linha<<endl;
}
