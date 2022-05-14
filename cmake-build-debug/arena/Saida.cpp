//
// Created by rafael on 13/05/22.
//

#include "Saida.h"

Saida::Saida(string arquivo) {
    this->arquivo="Saidas/"+arquivo;
    abrirArquivo();
    iniciado=false;
}

Saida::~Saida() {
    fecharArquivo();
}

void Saida::abrirArquivo() {
    this->saida.open(this->arquivo,ios::app);
}

void Saida::fecharArquivo() {
    if (this->saida.is_open()){
        this->saida.close();
    }
}

void Saida::imprimirResultado(Problema *problema, double tempo) {

    this->saida<<to_string(problema->funcaoObjetivo)<<" "<<to_string(tempo)<<endl;
}

void Saida::tokenize(string str, vector<string> &token_v, string DELIMITER) {
    size_t start = str.find_first_not_of(DELIMITER), end=start;

    while (start != std::string::npos) {
        end = str.find(DELIMITER, start);
        token_v.push_back(str.substr(start, end-start));
        start = str.find_first_not_of(DELIMITER, end);
    }
}