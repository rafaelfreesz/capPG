//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_TREE_H
#define CAPHH_TREE_H
#include <vector>
#include "Node.h"
#include "Grammar.h"
#include "Configures.h"
#include <sstream>
#define NONTERMINAL -1
class Tree {
public:
    Tree(Configures* conf, Grammar* grammar);
    Tree(Configures* conf, Grammar* grammar, Node* n);
    ~Tree();

    string str();
    Tree* clone();
    Node* clone(Node* n);
    Node* subTree();
    Node* targetSubTree(Node* n);
    void targetedNonTerminalCount(Node* n, Node* m);
    void update();

    void update (Node* n);
    Configures* conf;

    void print();

    Grammar* grammar;
    vector<Node*> nonTerminals;
    vector<Node*> targetedNonTerminals;
    double fitness;


    Node* root;
};


#endif //CAPHH_TREE_H
