//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_ARVORE_H
#define CAPHH_ARVORE_H
#include <vector>
#include "No.h"
#include "Gramatica.h"
#include "Configuracoes.h"
#include <sstream>
#define NONTERMINAL -1
class Arvore {
public:
    Arvore(Configuracoes* conf, Gramatica* grammar);
    Arvore(Configuracoes* conf, Gramatica* grammar, No* n);
    ~Arvore();
    string str();
    Arvore* clone();
    No* clone(No* n);
    No* subTree();
    No* targetSubTree(No* n);
    void targetedNonTerminalCount(No* n, No* m);
    void print();

    void update();
    void update (No* n);

    Configuracoes* conf;
    Gramatica* gramatica;
    vector<No*> nonTerminals;
    vector<No*> targetedNonTerminals;
    double fitness;
    No* raiz;
};


#endif //CAPHH_ARVORE_H
