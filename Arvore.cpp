//
// Created by rafael on 22/02/2022.
//

#include "Arvore.h"

Arvore::Arvore(Configuracoes* conf, Gramatica* grammar) {
    this->conf=conf;
    this->gramatica=grammar;

    No* n = new No(-1, 0, "<do>",0);

    if(rand()%2==0){
        this->gramatica->derivate(n);
    }else{
        int profundidade= 4 + (rand() % (this->conf->profMaxima - 3));
        this->gramatica->criarPopulacaoInicial(n, profundidade);
    }

    this->raiz=n;
    this->fitness=0;
    update();

}
//Segundo construtor
Arvore::Arvore(Configuracoes* conf, Gramatica* grammar, No* n){
    this->conf=conf;
    this->gramatica=grammar;
    this->raiz = n;
    this->fitness=0;
}
Arvore::~Arvore() {
    this->nonTerminals.clear();
    this->targetedNonTerminals.clear();
}
string Arvore::str() {
    stringstream model;
    this->raiz->str(model);
    return model.str();
}

void Arvore::print() {
    this->raiz->print();
}

Arvore* Arvore::clone() {
    No* n = this->clone(raiz);
    Arvore* t=new Arvore(this->conf, this->gramatica, n);
    t->update();

    return t;
}

No* Arvore::clone(No *n) {
    No* m = n->clone();

    for(No* f : n->sons) {
        m->addSon(clone(f));
    }

    return m;
}

void Arvore::update() {
    this->nonTerminals.clear();
    update(this->raiz);
}
//Atualiza altura da arvore
void Arvore::update(No *n) {
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

No *Arvore::subTree() {
    return this->nonTerminals.at(rand() % this->nonTerminals.size());
}

No* Arvore::targetSubTree(No *n) {
    targetedNonTerminalCount(n, this->raiz);
    if(targetedNonTerminals.size() > 0)
        return this->targetedNonTerminals.at(rand() % this->targetedNonTerminals.size());
    return NULL;
}

void Arvore::targetedNonTerminalCount(No *n, No *m) {
    if(n->type == m->type &&
       n->value == m->value &&
       n->deep + m->height < this->conf->profMaxima &&
       m->deep + n->height < this->conf->profMaxima)
        targetedNonTerminals.push_back(m);
    for(No* p : m->sons)
        targetedNonTerminalCount(n, p);

}