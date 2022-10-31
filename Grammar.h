//
// Created by rafael on 21/02/2022.
//

#ifndef CAPHH_GRAMMAR_H
#define CAPHH_GRAMMAR_H
#include <tuple>
#include <vector>
#include <fstream>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "Configures.h"
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

    Node(){ this->visited=false; this->high=-1;}
};

class Grammar {
public:
    Grammar(string gram, Configures* conf);
    virtual ~Grammar();

    vector<Node*> grammar;
    vector<string> nonTerminals;
    vector<vector<string>> terminals;
    Configures* conf;

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
    int buildInitialPopulation(No* n, int deep);

    void printGrammar();
    void printTerminals();
};


#endif //CAPHH_GRAMMAR_H
