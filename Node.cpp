//
// Created by rafael on 22/02/2022.
//

#include "Node.h"

//Constructor
Node::Node(double type, double value, string mask, int deep) {
    this->type=type;
    this->value=value;
    this->mask=mask;
    this->deep=deep;
}

//Insert node n as son
void Node::addSon(Node *n) {
    this->sons.push_back(n);
}

//Return a subtree as string
void Node::str(ostream &model) {

    if(this->type != -1) {

        model << " " << this->mask;


        if (this->mask == ";") {
            model << endl;
        }
    }

    for(Node* n : this->sons){
        n->str(model);
    }

}

//print subtree
void Node::print() {
    if(type != -1)
        cout << mask << " ";

    for(Node* n : sons)
        n->print();
}

//Return node clone
Node *Node::clone() {
    Node* n = new Node(this->type, this->value, this->mask, this->deep);
    return n;
}

//Erase tree
void Node::erase() {
    for(Node* n : this->sons)
        delete n;
    this->sons.clear();
}
