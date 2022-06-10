//
// Created by rafael on 13/02/2022.
//

#ifndef CAPHH_SAIDA_H
#define CAPHH_SAIDA_H
#include <fstream>
#include <cstring>
#include "Individuo.h"

using namespace std;
class Saida {
public:
    Saida(string arquivo);
    ~Saida();
    void abrirArquivo();
    void fecharArquivo();
    void imprimirResultado(Individuo **pop, int popSize, int generation, int generationSeed);
    void escreverLinha(string linha);

private:
    string arquivo;
    ofstream saida;
};


#endif //CAPHH_SAIDA_H
