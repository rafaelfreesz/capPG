//
// Created by rafael on 22/02/2022.
//

#include "Tree.h"

//Derivator Constructor
Tree::Tree(Configures* conf, Grammar* grammar) {
    this->conf=conf;
    this->grammar=grammar;

    No* n = new No(-1, 0, "<do>", 0);

    if(rand()%2==0){
        this->grammar->derivate(n);
    }else{
        int deep= 4 + (rand() % (this->conf->maxDeep - 3));
        this->grammar->buildInitialPopulation(n, deep);
    }

    this->root=n;
    this->fitness=0;
    update();

}

//Basic constructor
Tree::Tree(Configures* conf, Grammar* grammar, No* n){
    this->conf=conf;
    this->grammar=grammar;
    this->root = n;
    this->fitness=0;
}

//Destructor
Tree::~Tree() {
    this->nonTerminals.clear();
    this->targetedNonTerminals.clear();
}

//Returns a single string with all nodes in the tree
string Tree::str() {
    stringstream model;
    this->root->str(model);
    return model.str();
}

//Prints the tree on the screen
void Tree::print() {
    this->root->print();
}

//Returns a clone of the tree
Tree* Tree::clone() {
    No* n = this->clone(root);
    Tree* t=new Tree(this->conf, this->grammar, n);
    t->update();

    return t;
}

//Returns a clone of a node
No* Tree::clone(No *n) {
    No* m = n->clone();

    for(No* f : n->sons) {
        m->addSon(clone(f));
    }

    return m;
}

//Updating tree height
void Tree::update() {
    this->nonTerminals.clear();
    update(this->root);
}

//Updating tree height from a gived node
void Tree::update(No *n) {
    if(n->type == NONTERMINAL) {
        this->nonTerminals.push_back(n);
        n->height = 0;

        for(No* m : n->sons) {
            m->deep = n->deep + 1;
            update(m);

            if(m->height + 1 > n->height)
                n->height = m->height + 1;
        }
    } else {
        n->height = 0;
    }

}

//Returns a random subtree
No *Tree::subTree() {
    return this->nonTerminals.at(rand() % this->nonTerminals.size());
}

//Returns a subtree from a gived node
No* Tree::targetSubTree(No *n) {
    targetedNonTerminalCount(n, this->root);
    if(targetedNonTerminals.size() > 0)
        return this->targetedNonTerminals.at(rand() % this->targetedNonTerminals.size());
    return nullptr;
}

//calculates the number of non-terminals
void Tree::targetedNonTerminalCount(No *n, No *m) {
    if(n->type == m->type &&
       n->value == m->value &&
       n->deep + m->height < this->conf->maxDeep &&
       m->deep + n->height < this->conf->maxDeep) {
        targetedNonTerminals.push_back(m);
    }

    for(No* p : m->sons)
        targetedNonTerminalCount(n, p);

}