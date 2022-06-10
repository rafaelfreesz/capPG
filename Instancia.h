//
// Created by rafael on 13/05/22.
//

#ifndef CAPHH_INSTANCIA_H
#define CAPHH_INSTANCIA_H
#include <iostream>
#include <fstream>
using namespace std;

class Instancia {

public:
    Instancia ();
    ~Instancia();

    //Elementos da instancia
    string nome;
    int n;
    float litSol;
    int* comprimentos;
    int** demandas;

    //Elementos da solucao base
    int particao;
    float funcaoObjetivo;
    int* corredor;
    int* abcissas;


    void alocarComprimentos();
    void alocarDemandas();
    void gerarSolucaoInicial();

    void imprimirComprimentos();
    void imprimirDemandas();
    void imprimirLayout();

    void swapIndex(int i, int j);
    void calcularSolucao();

};


#endif //CAPHH_INSTANCIA_H
