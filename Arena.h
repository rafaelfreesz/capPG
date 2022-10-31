//
// Created by rafael on 08/06/22.
//
#include <cmath>
#include <vector>
#include "Instance.h"

#ifndef CAPHH_ARENA_H
#define CAPHH_ARENA_H


class Arena {

public:

    Arena(Instance *instancia);
    ~Arena();

    void go(vector <string>* individuoTokenizado);
    void interpretar(vector <string>* individuoLinear);

    void calcularSolucao();
    void montarAbcissas();
    void calcularCusto();
    void importarSolucao();
    void swapIndex(int i, int j);

    //operacoes//
    //pert//
    void swap(float iF, float jF);
    void repart(float fatorF);
    void shake (float tamF);
    void rec(float tam);

    void insert(int iOrigem, int iDestino);
    //ref//
    void RVND();
    void bL1();
    void bL2();
    void bL3();
    void bL4();
    void blPc();

    int giveMeIndex(float value);

    void bLFase1(bool* melhorado);
    void bLFase2(bool* melhorado);
    void bLFase3(bool* melhorado);
    void bLFase4(bool* melhorado);


    //Atributos
    Instance* instancia;
    int* corredor;
    float* abcissas;
    int p;
    float fo;
    int n;

    void imprimirCorredor();
};


#endif //CAPHH_ARENA_H
