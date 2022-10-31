//
// Created by rafael on 13/02/2022.
//

#include "Output.h"

//Constructor
Output::Output(string file) {
    this->fileName=file;
    openFile();
}

//Destructor
Output::~Output() {
    closeFile();
}

//Open file
void Output::openFile() {
    this->outputFile.open(this->fileName);
}

//Close file
void Output::closeFile() {
    if (this->outputFile.is_open()){
        this->outputFile.close();
    }
}

//Print results to output file
void Output::printResults(Individual **pop, int popSize, int generation, int generationSeed) {
    this->outputFile << to_string(generation) << " | " << to_string(generationSeed) << " | ";
    for(int i=0;i<popSize;i++){
        this->outputFile << pop[i]->fitness << " (" << pop[i]->name << ") - ";
    }
    this->outputFile << endl;
}

void Output::printLine(string line) {
    this->outputFile << line << endl;
}
