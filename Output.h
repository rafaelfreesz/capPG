//
// Created by rafael on 13/02/2022.
//

#ifndef CAPHH_OUTPUT_H
#define CAPHH_OUTPUT_H
#include <fstream>
#include <cstring>
#include "Individual.h"

using namespace std;
class Output {
public:
    Output(string file);
    ~Output();
    void openFile();
    void closeFile();
    void printResults(Individual **pop, int popSize, int generation, int generationSeed);
    void printLine(string line);

private:
    string fileName;
    ofstream outputFile;
};


#endif //CAPHH_OUTPUT_H
