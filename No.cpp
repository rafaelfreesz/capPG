//
// Created by rafael on 22/02/2022.
//

#include "No.h"

No::No(double type, double value, string mask, int deep) {
    this->type=type;
    this->value=value;
    this->mask=mask;
    this->deep=deep;
}

void No::addSon(No *n) {
    this->sons.push_back(n);
}

void No::str(ostream &model) {

    if(this->type != -1) {

        model << " " << this->mask;


        if (this->mask == ";") {
            model << endl;
        }
    }

    for(No* n : this->sons){
        n->str(model);
    }

}

void No::print() {
    if(type != -1)
        cout << mask << " ";
    //cout << "[" << mask << "," << type << "] ";

    for(No* n : sons)
        n->print();
}

No *No::clone() {
    No* n = new No(this->type, this->value, this->mask, this->deep);
    return n;
}

void No::erase() {
    for(No* n : this->sons)
        delete n;
    this->sons.clear();
}
