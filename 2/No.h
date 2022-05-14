//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_NO_H
#define CAPHH_NO_H
#include <iostream>
#include <vector>
#include <ctype.h>

using namespace std;
class No {
public:
    No(double type, double value, string mask, int deep);

    void addSon(No* n);
    void str(ostream& model);
    No* clone();
    void print();
    void erase();

    double type;
    double value;
    string mask;
    int deep;
    int height;
    int index;
    vector<No*> sons;
    No* father;
};


#endif //CAPHH_NO_H
