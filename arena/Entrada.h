//
// Created by rafael on 13/05/22.
//

#ifndef ARENAPG_ENTRADA_H
#define ARENAPG_ENTRADA_H

#include <iostream>
#include <cstring>
#include "Problema.h"
#include "Sala.h"
using namespace std;

class Entrada {
public:
    Entrada(string arquivo);
    ~Entrada();

    Problema* montarProblema();
    char* proximaLinha();
    string proximaInstancia();
    char* stringToChar(string linha);
    static string charToString(char* linha);

private:
    void abrirArquivo();
    void fecharArquivo();
    Sala** montarVetorDeSalas(int qtdSalas);
    int** montarMatrizDeTrafego(int qtdSalas);

private:
    FILE* file;
    string arquivo;
};


#endif //ARENAPG_ENTRADA_H
