//
// Created by rafael on 13/05/22.
//

#ifndef ARENAPG_PROBLEMA_H
#define ARENAPG_PROBLEMA_H

#include <iostream>
#include <ctime>
#include "Sala.h"
using namespace std;

class Problema {
public:
    //Funções
    Problema(int qtdSalas);
    ~Problema();

    void go();

    void calcularSolucao();
    void montarAbcissas();
    void calcularCusto();
    void resetarProblema();
    void construir();

    //operacoes//
    //pert//
    void swap(int i,int j);
    void repart(int fator);
    void shake (int tam);
    void rec(int tam);

    void insert(int iOrigem, int iDestino);
    //ref//
    void RVND();
    void bL1();
    void bL2();
    void bL3();
    void bL4();
    void blPc();



    void bLFase1(bool* melhorado);
    void bLFase2(bool* melhorado);
    void bLFase3(bool* melhorado);
    void bLFase4(bool* melhorado);

    //Atributos
    Sala** vetorDeSalas;
    int** matrizDeTrafego;
    int* corredorSolucao;
    float* abcissas;

    int n;
    int particao;
    float funcaoObjetivo;

    void imprimirCorredor();
private:
};


#endif //ARENAPG_PROBLEMA_H
