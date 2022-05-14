//
// Created by rafael on 22/02/2022.
//

#include "Tree.h"

Tree::Tree(Configures* conf, Grammar* grammar) {
    this->conf=conf;
    this->grammar=grammar;

    No* n = new No(-1, 0, "<bigInit>",0);

    if(rand()%2==0){
        this->grammar->derivate(n);
    }else{
        int deep= 4+(rand()%(this->conf->maxDeep-3));

        this->grammar->createInitialPopulation(n, deep);
    }

    this->root=n;
    this->fitness=0;
    update();


}
//Segundo construtor
Tree::Tree(Configures* conf, Grammar* grammar, No* n){
    this->conf=conf;
    this->grammar=grammar;
    this->root = n;
    this->fitness=0;
}
Tree::~Tree() {
    this->nonTerminals.clear();
    this->targetedNonTerminals.clear();
}
string Tree::str() {
    stringstream model;
    this->root->str(model);
    return model.str();
}

void Tree::print() {
    this->root->print();
}

Tree* Tree::clone() {
    No* n = this->clone(root);
    Tree* t=new Tree(this->conf,this->grammar,n);
    t->update();

    return t;
}

No* Tree::clone(No *n) {
    No* m = n->clone();

    for(No* f : n->sons) {
        m->addSon(clone(f));
    }

    return m;
}

void Tree::update() {
    this->nonTerminals.clear();
    update(this->root);
}
//Atualiza altura da arvore
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

No *Tree::subTree() {
    return this->nonTerminals.at(rand() % this->nonTerminals.size());
}

No* Tree::targetSubTree(No *n) {
    targetedNonTerminalCount(n, this->root);
    if(targetedNonTerminals.size() > 0)
        return this->targetedNonTerminals.at(rand() % this->targetedNonTerminals.size());
    return NULL;
}

void Tree::targetedNonTerminalCount(No *n, No *m) {
    if(n->type == m->type &&
       n->value == m->value &&
       n->deep + m->height < this->conf->maxDeep &&
       m->deep + n->height < this->conf->maxDeep)
        targetedNonTerminals.push_back(m);
    for(No* p : m->sons)
        targetedNonTerminalCount(n, p);

}