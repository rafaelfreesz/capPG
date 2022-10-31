//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_TREE_H
#define CAPHH_TREE_H
#include <vector>
#include "No.h"
#include "Grammar.h"
#include "Configures.h"
#include <sstream>
#define NONTERMINAL -1
class Tree {
public:
    Tree(Configures* conf, Grammar* grammar);
    Tree(Configures* conf, Grammar* grammar, No* n);
    ~Tree();

    string str();
    Tree* clone();
    No* clone(No* n);
    No* subTree();
    No* targetSubTree(No* n);
    void targetedNonTerminalCount(No* n, No* m);
    void update();

    void update (No* n);
    Configures* conf;

    void print();

    Grammar* grammar;
    vector<No*> nonTerminals;
    vector<No*> targetedNonTerminals;
    double fitness;


    No* root;
};


#endif //CAPHH_TREE_H
