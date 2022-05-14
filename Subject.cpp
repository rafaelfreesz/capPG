//
// Created by rafael on 22/02/2022.
//

#include "Subject.h"

Subject::Subject(bool inicializa, Configures* conf, Grammar* grammar) {

    this->conf=conf;
    this->grammar=grammar;

    if(inicializa) {
        this->tree = new Tree(this->conf,this->grammar);
    }
}

Subject *Subject::clone() {
    Subject* clone = new Subject(false,this->conf,this->grammar);

    clone->fitness=this->fitness;
    clone->tree=this->tree->clone();

    return clone;
}
