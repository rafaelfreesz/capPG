//
// Created by rafael on 22/02/2022.
//

#include "No.h"

//Constructor
No::No(double type, double value, string mask, int deep) {
    this->type=type;
    this->value=value;
    this->mask=mask;
    this->deep=deep;
}

//Insert node n as son
void No::addSon(No *n) {
    this->sons.push_back(n);
}

//Return a subtree as string
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

//print subtree
void No::print() {
    if(type != -1)
        cout << mask << " ";

    for(No* n : sons)
        n->print();
}

//Return node clone
No *No::clone() {
    No* n = new No(this->type, this->value, this->mask, this->deep);
    return n;
}

//Erase tree
void No::erase() {
    for(No* n : this->sons)
        delete n;
    this->sons.clear();
}
