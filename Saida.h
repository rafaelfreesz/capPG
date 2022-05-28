//
// Created by rafael on 13/02/2022.
//

#ifndef CAPHH_SAIDA_H
#define CAPHH_SAIDA_H
#include <fstream>
#include <cstring>
#include "Subject.h"

using namespace std;
class Saida {
public:
    Saida(string arquivo);
    ~Saida();
    void abrirArquivo();
    void fecharArquivo();
    void imprimirResultado(Subject **pop, int popSize, int generation, int seed);
    void escreverLinha(string linha);

private:
    string arquivo;
    ofstream saida;
};


#endif //CAPHH_SAIDA_H
