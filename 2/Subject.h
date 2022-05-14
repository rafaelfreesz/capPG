//
// Created by rafael on 22/02/2022.
//

#ifndef CAPHH_SUBJECT_H
#define CAPHH_SUBJECT_H
#include "Tree.h"
#include "Configures.h"
#include "Grammar.h"

using namespace std;
class Subject {
public:
    Subject(bool inicializa, Configures* conf, Grammar* grammar);
    Subject* clone();

    double fitness;
    Tree* tree;
    Configures* conf;
    Grammar* grammar;
    string subjectName;
};


#endif //CAPHH_SUBJECT_H
