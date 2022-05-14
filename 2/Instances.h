//
// Created by rafael on 13/05/22.
//

#ifndef CAPHH_INSTANCES_H
#define CAPHH_INSTANCES_H
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Instances {

public:
    Instances ();
    ~Instances();

    vector<string> names;
    vector<int> nFacilities;
    vector<float> litSol;
    int n;

    void tokenize(string str, vector<string> &token_v, string DELIMITER);
    void getNfacilities(string instance);

};


#endif //CAPHH_INSTANCES_H
