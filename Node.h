//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_NODE_H
#define CAPHH_NODE_H
#include <iostream>
#include <vector>
#include <ctype.h>

using namespace std;
class Node {
public:
    Node(double type, double value, string mask, int deep);

    void addSon(Node* n);
    void str(ostream& model);
    Node* clone();
    void print();
    void erase();

    double type;
    double value;
    string mask;
    int deep;
    int height;
    int index;
    vector<Node*> sons;
    Node* father;
};


#endif //CAPHH_NODE_H
