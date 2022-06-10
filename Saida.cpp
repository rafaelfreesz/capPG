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

void Saida::imprimirResultado(Individuo **pop, int popSize, int generation, int generationSeed) {
    this->saida << to_string(generation) << " | " << to_string(generationSeed) << " | ";
    for(int i=0;i<popSize;i++){
        this->saida << pop[i]->fitness << " (" << pop[i]->nome << ") - ";
    }
    this->saida<<endl;
}

void Saida::escreverLinha(string linha) {
    this->saida<<linha<<endl;
}
