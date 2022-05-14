//
// Created by rafael on 13/05/22.
//

#include "Instances.h"

Instances::Instances() {
    string lineInst="lit.sol";

    ifstream instList, solList;
    instList.open(lineInst.c_str());

    while(getline(instList,lineInst)){

        vector<string> tokens;
        tokenize(lineInst,tokens,";");

        this->names.push_back(tokens.at(0));
        this->litSol.push_back(stof(tokens.at(1)));

        getNfacilities(tokens.at(0));

    }
    if(this->names.size() == this->litSol.size()) {
        this->n=this->names.size();
    }else{
        cout<<"Erro! Numero de instancias não batem"<<endl;
    }

    instList.close();
    solList.close();


}
void Instances::getNfacilities(string instance) {

    string path="arena/Instancias/"+instance;
    ifstream instStream;
    instStream.open(path.c_str());

    string line;
    getline(instStream,line);
    this->nFacilities.push_back(stoi(line));

    instStream.close();
}

void Instances::tokenize(string str, vector<string> &token_v, string DELIMITER) {
    size_t start = str.find_first_not_of(DELIMITER), end=start;

    while (start != std::string::npos) {
        end = str.find(DELIMITER, start);
        token_v.push_back(str.substr(start, end-start));
        start = str.find_first_not_of(DELIMITER, end);
    }
}

