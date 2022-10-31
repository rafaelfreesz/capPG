//
// Created by rafael on 22/02/2022.
//

#include "Tree.h"

//Derivator Constructor
Tree::Tree(Configures* conf, Grammar* grammar) {
    this->conf=conf;
    this->grammar=grammar;

    Node* n = new Node(-1, 0, "<do>", 0);

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
Tree::Tree(Configures* conf, Grammar* grammar, Node* n){
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
    Node* n = this->clone(root);
    Tree* t=new Tree(this->conf, this->grammar, n);
    t->update();

    return t;
}

//Returns a clone of a node
Node* Tree::clone(Node *n) {
    Node* m = n->clone();

    for(Node* f : n->sons) {
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
void Tree::update(Node *n) {
    if(n->type == NONTERMINAL) {
        this->nonTerminals.push_back(n);
        n->height = 0;

        for(Node* m : n->sons) {
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
Node *Tree::subTree() {
    return this->nonTerminals.at(rand() % this->nonTerminals.size());
}

//Returns a subtree from a gived node
Node* Tree::targetSubTree(Node *n) {
    targetedNonTerminalCount(n, this->root);
    if(targetedNonTerminals.size() > 0)
        return this->targetedNonTerminals.at(rand() % this->targetedNonTerminals.size());
    return nullptr;
}

//calculates the number of non-terminals
void Tree::targetedNonTerminalCount(Node *n, Node *m) {
    if(n->type == m->type &&
       n->value == m->value &&
       n->deep + m->height < this->conf->maxDeep &&
       m->deep + n->height < this->conf->maxDeep) {
        targetedNonTerminals.push_back(m);
    }

    for(Node* p : m->sons)
        targetedNonTerminalCount(n, p);

}