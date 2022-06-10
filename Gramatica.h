//
// Created by rafael on 21/02/2022.
//

#ifndef CAPHH_GRAMATICA_H
#define CAPHH_GRAMATICA_H
#include <tuple>
#include <vector>
#include <fstream>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "Configuracoes.h"
#include "No.h"
#define INFINT 2147483647
using namespace std;

class Node{
public:
    vector<Node*> productions;
    double type;
    double value;
    string mask;
    bool visited;
    int high;

    Node(){this->visited=false; this->high=-1;}
};

class Gramatica {
public:
    Gramatica(string gram, Configuracoes* conf);
    virtual ~Gramatica();

    vector<Node*> gramatica;
    vector<string> naoTerminais;
    vector<vector<string>> terminais;
    Configuracoes* conf;

    void tokenize(string str, vector<string> &token_v, string DELIMITER);
    void makeTerminals();
    bool isNonTerminal(string s);
    double getNonTerminal(string nt);
    tuple<double, double, string> getTerminal(string t);
    tuple<double, double, string> getString(string s);
    void updateHigh();
    void high1(Node* n);
    void high2();
    static bool sortNode(Node* a, Node* b);
    int derivate(No* n);
    int criarPopulacaoInicial(No* n, int deep);

    void printGrammar();
    void printProductions(Node* nonTerminalNode);
    void printTerminals();
};


#endif //CAPHH_GRAMATICA_H
