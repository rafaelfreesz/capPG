//
// Created by rafael on 13/05/22.
//

#ifndef ARENAPG_SAIDA_H
#define ARENAPG_SAIDA_H

#include <fstream>
#include <cstring>
#include <vector>
#include "Problema.h"
#include "math.h"
#include "Entrada.h"
using namespace std;

class Saida {
public:
    Saida(string arquivo);
    ~Saida();

    void abrirArquivo();
    void fecharArquivo();
    void imprimirResultado(Problema *problema, double tempo);
    void tokenize(string str, vector<string> &token_v, string DELIMITER);

private:
    string arquivo;
    ofstream saida;
    bool iniciado;
};

#endif //ARENAPG_SAIDA_H
