//
// Created by rafael on 13/05/22.
//

#include "Instance.h"

//Constructor
Instance::Instance() {
    this->name="";
    this->literatureCost=-1.0;
    this->n=-1;
    this->cost=-1;
    this->partition=-1;
    this->lengths= nullptr;
    this->demands = nullptr;
    this->corridor= nullptr;
    this->abcissas= nullptr;
}

//Destructor
Instance::~Instance() {

    delete[] this->lengths;
    delete[] this->corridor;
    delete[] this->abcissas;
    for(int i=0;i<n;i++){
        delete[] this->demands[i];
    }
    delete[] this->demands;
}

//Build instance lengths
void Instance::buildLengths() {
    if(this->n!=-1){
        this->lengths=new int[this->n];
    }else{
        cout<<"Unknown n"<<endl;
        exit(1);
    }
}

//Build demand matrix
void Instance::buildDemands() {
    if(this->n!=-1){
        this->demands=new int*[this->n];
        for(int i=0;i<this->n;i++){
            this->demands[i]=new int[this->n];
        }
    }else{
        cout<<"Unknown n"<<endl;
        exit(1);
    }
}

//Build inicial reference solution
void Instance::buildInitialSolution() {

    if(this->n!=-1){
        if (this->corridor == nullptr) {
            this->corridor = new int[this->n];
            this->abcissas = new int[this->n];
        }
    }else{
        cout<<"Unknown n"<<endl;
        exit(1);
    }

    for(int i=0;i<this->n;i++){
        this->corridor[i]=i;
    }
    this->partition= (this->n / 2 - 2) + rand() % 5;
    for(int i=0;i<this->n;i++){
        int j=rand()%this->n;
        int k=rand()%this->n;
        while(k==j){
            k=rand()%this->n;
        }
        swapIndex(i,j);
    }
    calculateSolution();
}

//Print instance lengths
void Instance::printLengths() {
    if(this->n!=-1) {
        for (int i = 0; i <this->n;i++){
            cout << this->lengths[i] << " ";
        }
        cout<<endl;
    }else{
        cout<<"Unknown n"<<endl;
        exit(1);
    }
}

//Print instance demands
void Instance::printDemands() {
    if(this->n!=-1) {
        for (int i = 0; i <this->n;i++){
            for(int j=0;j<this->n;j++){
                cout << this->demands[i][j] << " ";
            }
            cout<<endl;
        }
        cout<<endl;
    }else{
        cout<<"Unkown n"<<endl;
        exit(1);
    }
}

//Print layout
void Instance::printLayout() {
    if(this->n!=-1) {
        for (int i = 0; i <this->n;i++){
            cout << this->corridor[i] << " ";
        }
        cout<<endl;
    }else{
        cout<<"Unknown n"<<endl;
        exit(1);
    }
}

//Swap facilites from 2 gived indexes
void Instance::swapIndex(int i, int j) {
    int aux=this->corridor[i];
    this->corridor[i]=this->corridor[j];
    this->corridor[j]=aux;
}

//Calculate Solution of a gived layout
void Instance::calculateSolution() {
    //Calculating Abcissa
    int sizeLength=0;

    int facility=-1;
    int length=-1;

    for(int i=0;i<this->n;i++){
        if(i==this->partition){
            sizeLength=0;
        }

        facility=this->corridor[i];
        length=this->lengths[facility];
        this->abcissas[facility]= sizeLength + (float)length / 2;
        sizeLength+=length;
    }

    //Calculating cost
    this->cost=0;

    for(int i=0; i < (this->n - 1); i++){
        int facI=this->corridor[i];
        for(int j=i+1;j<this->n;j++){
            int facJ=this->corridor[j];
            float distance=abs(abcissas[facJ] - abcissas[facI]);
            float localCost= distance * (float)this->demands[facI][facJ];
            this->cost+=localCost;
        }
    }
}






