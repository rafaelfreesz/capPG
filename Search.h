//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_SEARCH_H
#define CAPHH_SEARCH_H
#include <iostream>
#include <vector>
#include "Individuo.h"
#include "Gramatica.h"
#include "Configuracoes.h"
#include "Saida.h"
#include "Instancia.h"
#include "Arena.h"
#include <ctime>

using namespace std;
class Search {
public:
    Search(Configuracoes *conf, Gramatica *grammar, int seed);
    ~Search();

    void inciar();
    void iniciarPosTeste();
    void criarPopulacaoInicial();
    void avaliarPopulacao(int initialIndex, int finalIndex, int generation);
    void avaliarIndividuo(int index);
    static bool sortPopulationFitness(Individuo* a, Individuo* b);
    void randPais(Individuo **pais, int ind);
    void operate();
    void replace();

    //-----Elementos de Crossover-----//
    void crossover(Individuo** selecionados);
    void treeCrossover(Arvore* a, Arvore* b);

    //-----Elementos de Mutação-----//
    void mutate(Individuo** selecionados);
    void auxMutate(Arvore* t);

    //-----Preparações-----//
    void montarInstancias();
    void exportarResultadoIndividuo(int index);
    void exportarIndividuo(int index);
    void gerarSolucoesIniciais();

    void tokenize(string str, vector<string> &token_v, string DELIMITER);

    Individuo** populacao;
    Configuracoes* conf;
    Gramatica* grammar;
    string command;
    vector<Instancia> instancias;
    int seed;
};


#endif //CAPHH_SEARCH_H
