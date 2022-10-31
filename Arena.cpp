//
// Created by rafael on 08/06/22.
//
#include "Arena.h"
//Constructor
Arena::Arena(Instance *instancia) {

    this->instance=instancia;
    this->p=0;
    this->cost=0;
    this->corridor=new int[this->instance->n];
    this->abcissa=new float[this->instance->n];
    this->n=this->instance->n;
}
//Destructor
Arena::~Arena() {
    delete[] this->corridor;
    delete[] this->abcissa;
}

void Arena::go(vector<string>* tokenizedIndividual) {

    importSolution();

    float bestCost=this->cost;
    int bestP=this->p;
    int* bestCorridor=new int[this->instance->n];

    for(int i=0;i<50;i++) {

        interpretGrammar(tokenizedIndividual);

        if(this->cost < bestCost){
            bestCost=this->cost;
            bestP=this->p;
            for(int j=0;j<this->instance->n; j++){
                bestCorridor[j]=this->corridor[j];
            }
        }
    }

    this->cost=bestCost;
    this->p=bestP;
    for(int i=0;i<this->instance->n; i++){
        this->corridor[i]=bestCorridor[i];
    }
    delete[] bestCorridor;
}

//interprets the individual
void Arena::interpretGrammar(vector<string> *linearIndividual) {
    int i = 0;
    while (i < linearIndividual->size()) {

        if (linearIndividual->at(i) == "swap(") {
            swap(stof(linearIndividual->at(i + 1)), stof(linearIndividual->at(i + 3)));
            i += 5;
        } else if (linearIndividual->at(i) == "repart(") {
            string sentence = linearIndividual->at(i + 1) + linearIndividual->at(i + 2);
            repart(stof(sentence));
            i += 4;
        } else if (linearIndividual->at(i) == "shake(") {
            shake(stof(linearIndividual->at(i + 1)));
            i += 3;
        } else if (linearIndividual->at(i) == "rec(") {
            rec(stof(linearIndividual->at(i + 1)));
            i += 3;
        } else if (linearIndividual->at(i) == "RVND();") {
            RVND();
            i++;
        } else if (linearIndividual->at(i) == "bL1();") {
            bL1();
            i++;
        } else if (linearIndividual->at(i) == "bL2();") {
            bL2();
            i++;
        } else if (linearIndividual->at(i) == "bL3();") {
            bL3();
            i++;
        } else if (linearIndividual->at(i) == "bL4();") {
            bL4();
            i++;
        } else {
            blPc();
            i++;
        }
    }

    if (i > linearIndividual->size()) {
        cout << "pattern match error" << endl;
        exit(2);
    }

}

//Import base solution from Instance
void Arena::importSolution() {

    for(int i=0;i<this->instance->n; i++){
        this->corridor[i]=this->instance->corridor[i];
        this->abcissa[i]=this->instance->abcissas[i];
    }
    this->p=this->instance->partition;
    this->cost=this->instance->cost;

}

//Calculate layout abcissa
void Arena::calculateAbcissa() {
    int sizeLength=0;

    int facility=-1;
    int length=-1;

    for(int i=0;i<this->n;i++){
        if(i==this->p){
            sizeLength=0;
        }

        facility=this->corridor[i];
        length=this->instance->lengths[facility];
        this->abcissa[facility]= sizeLength + (float)length / 2;
        sizeLength+=length;
    }
}
//Calculate layout cost
void Arena::calculateCost() {
    this->cost=0;

    for(int i=0; i < (this->n - 1); i++){
        int facI=this->corridor[i];
        for(int j=i+1;j<this->n;j++){
            int facJ=this->corridor[j];
            float distance=abs(abcissa[facJ] - abcissa[facI]);
            float localCost= distance * (float)this->instance->demands[facI][facJ];
            this->cost+=localCost;
        }
    }
}
//Calculate Abcissa and Cost
void Arena::calculateSolution() {
    calculateAbcissa();
    calculateCost();
}

void Arena::printLayout() {
    cout<<endl;
    for(int i=0;i<this->n;i++){
        if(i==this->p){
            cout<<endl;
        }
        cout << this->corridor[i] << " ";
    }
    cout<<endl;
}

//Swap two facilities
void Arena::swap(float iF, float jF) {
    int i= giveMeIndex(iF);
    int j= giveMeIndex(jF);
    if(i < this->instance->n && j < this->instance->n) {
        int aux = this->corridor[i];
        this->corridor[i] = this->corridor[j];
        this->corridor[j] = aux;
    }
    calculateSolution();
}

//Repartition pert operator
void Arena::repart(float fFactor) {
    int factor= giveMeIndex(fFactor);
    if((this->p + factor) < this->instance->n && (this->p + factor) > 0) {
        this->p += factor;
        calculateSolution();
    }
}

//Shake pert operator
void Arena::shake(float sSizeRate) {

    int shakeSize= giveMeIndex(sSizeRate);
    if(shakeSize > 0){

        for(int i=0; i < shakeSize; i++){
            int j=rand()%this->instance->n;
            int k=rand()%this->instance->n;
            while(k==j){
                k=rand()%this->instance->n;
            }
            swapIndex(j,k);
        }
        calculateSolution();
    }
}

//Reconstruction Pert operator
void Arena::rec(float rSizeRate){
    int size= giveMeIndex(rSizeRate);

    if(size < this->instance->n) {
        int* deallocated= new int [size];

        
        for (int i = 0; i < size; i++) {
            int j = rand() % this->n;
            deallocated[i] = corridor[j];

            this->n--;

            for (int k = j; k < this->n; k++) {
                this->corridor[k] = this->corridor[k + 1];
            }

            if (j < this->p) {
                this->p--;
            }
        }
        bool left=((this->p) < (this->n - this->p));
        for (int i = 0; i < size; i++) {

            this->corridor[this->n] = deallocated[i];
            if(left){
                for(int j=this->n-1;j>=this->p;j--){
                    swapIndex(j,j+1);
                }
                this->p++;
            }
            left=!left;
            this->n++;
        }


        calculateSolution();
        delete[] deallocated;
    }
}

//RVND ref operator
void Arena::RVND() {
    bool improved=true;

    int sequenceArray[4]={0, 1, 2, 3};
    int phase=0;

    while(phase < 4){
        improved=false;

        if(phase == 0){
            for(int i=0;i<4;i++){
                int j=rand()%4;
                int k=rand()%4;
                while(k==j){
                    k=rand()%4;
                }
                int aux=sequenceArray[j];
                sequenceArray[j]=sequenceArray[k];
                sequenceArray[k]=aux;
            }
        }

        switch (sequenceArray[phase]) {
            case 0:
                bLPhase1(&improved);
                if(improved){
                    phase=0;
                }else{
                    phase++;
                }
                break;
            case 1:
                bLPhase2(&improved);
                if(improved){
                    phase=0;
                }else{
                    phase++;
                }
                break;
            case 2:
                bLPhase3(&improved);
                if(improved){
                    phase=0;
                }else{
                    phase++;
                }
                break;
            case 3:
                bLPhase4(&improved);
                if(improved){
                    phase=0;
                }else{
                    phase++;
                }
                break;
        }

    }
}

//RVND phase 1 local search
void Arena::bLPhase1(bool *improved) {
    float bestCost=this->cost;

    for(int i=1;i<this->p; i++){
        swapIndex(i-1,i);
        calculateSolution();

        if(this->cost < bestCost){
            *improved=true;
            return;
        }else{
            swapIndex(i-1,i);
            calculateSolution();
        }
    }
    for(int i= this->p + 1; i < this->instance->n; i++){
        swapIndex(i-1,i);
        calculateSolution();

        if(this->cost < bestCost){
            *improved=true;
            return;
        }else{
            swapIndex(i-1,i);
            calculateSolution();
        }
    }
}
//RVND phase 2 local search
void Arena::bLPhase2(bool *improved) {
    float bestCost=this->cost;

    for(int i=0;i<(this->p - 2); i++){

        for(int j=i+2;j<this->p; j++) {
            swapIndex(i , j);
            calculateSolution();

            if (this->cost < bestCost) {
                *improved = true;
                return;
            } else {
                swapIndex(i , j);
                calculateSolution();
            }
        }
    }

    for(int i=this->p; i < (this->instance->n - 2); i++){

        for(int j=i+2;j<this->instance->n; j++) {
            swapIndex(i , j);
            calculateSolution();

            if (this->cost < bestCost) {
                *improved = true;
                return;
            } else {
                swapIndex(i , j);
                calculateSolution();
            }
        }
    }
}
//RVND phase 3 local search
void Arena::bLPhase3(bool *improved) {
    float bestCost=this->cost;
    for(int i=0;i<this->p; i++){
        for(int j=this->p; j < this->instance->n; j++){
            swapIndex(i, j);
            calculateSolution();

            if (this->cost < bestCost) {
                *improved = true;
                return;
            } else {
                swapIndex(i, j);
                calculateSolution();
            }
        }
    }
}
//RVND phase 4 local search
void Arena::bLPhase4(bool *improved) {
    float bestCost=this->cost;

    for (int i = 0; i < this->p; i++) {

        this->p--;
        insert(i, this->instance->n - 1);
        calculateSolution();

        if (this->cost < bestCost) {
            *improved = true;
            return;
        }

        for (int j = this->instance->n - 1; j > this->p; j--) {
            swapIndex(j, j-1);
            calculateSolution();

            if (this->cost < bestCost) {
                *improved = true;
                return;
            }
        }
        this->p++;
        insert(this->p - 1, i);
        calculateSolution();
    }



    for (int i = this->p; i < this->instance->n; i++) {

        this->p++;
        insert(i, this->p - 1);
        calculateSolution();

        if (this->cost < bestCost) {
            *improved = true;
            return;
        }

        for (int j = this->p - 1; j > 0; j--) {
            swapIndex(j, j - 1);
            calculateSolution();
            if (this->cost < bestCost) {
                *improved = true;
                return;
            }
        }
        this->p--;
        insert(0, i);
        calculateSolution();
    }

}

//Insert ref operator
void Arena::insert(int iSource, int iDestino) {
    if(iSource < this->instance->n && iSource >= 0 && iDestino < this->instance->n && iDestino >= 0 && iSource != iDestino) {

        int aux = this->corridor[iSource];
        if (iSource < iDestino) {

            for (int j = iSource; j < iDestino; j++) {
                this->corridor[j] = this->corridor[j + 1];
            }

        } else {

            for (int j = iSource; j > iDestino; j--) {
                this->corridor[j] = this->corridor[j - 1];
            }

        }

        this->corridor[iDestino] = aux;
    }
}

//bL1 ref operator
void Arena::bL1() {
    float bestCost=this->cost;

    for(int i=1;i<this->p; i++){
        swapIndex(i-1,i);
        calculateSolution();

        if(this->cost < bestCost){
            bestCost= this->cost;
            i=0;
        }else{
            swapIndex(i-1,i);
            calculateSolution();
        }
    }
    for(int i= this->p + 1; i < this->instance->n; i++){
        swapIndex(i-1,i);
        calculateSolution();

        if(this->cost < bestCost){
            bestCost= this->cost;
            i=this->p;
        }else{
            swapIndex(i-1,i);
            calculateSolution();
        }
    }
}

//bL2 ref operator
void Arena::bL2() {
    float bestCost=this->cost;

    for(int i=0;i<(this->p - 2); i++){

        for(int j=i+2;j<this->p; j++) {
            swapIndex(i , j);
            calculateSolution();

            if (this->cost < bestCost) {
                bestCost= this->cost;
                i=-1;
                break;
            } else {
                swapIndex(i , j);
                calculateSolution();
            }
        }
    }

    for(int i=this->p; i < (this->instance->n - 2); i++){

        for(int j=i+2;j<this->instance->n; j++) {
            swapIndex(i , j);
            calculateSolution();

            if (this->cost < bestCost) {
                bestCost= this->cost;
                i= this->p - 1;
                break;
            } else {
                swapIndex(i , j);
                calculateSolution();
            }
        }
    }
}

//bL3 ref operator
void Arena::bL3() {
    float bestCost=this->cost;

    for (int i = 0; i < this->p; i++) {
        for (int j = this->p; j < this->instance->n; j++) {
            swapIndex(i, j);
            calculateSolution();

            if (this->cost < bestCost) {
                bestCost=this->cost;
                i=-1;
                break;
            } else {
                swapIndex(i, j);
                calculateSolution();
            }
        }
    }


}

//bL4 ref operator
void Arena::bL4() {
    float bestCost=this->cost;

    bool improved=true;

    while(improved) {
        improved = false;
        for (int i = 0; i < this->p; i++) {

            this->p--;
            insert(i, this->instance->n - 1);
            calculateSolution();

            if (this->cost < bestCost) {
                bestCost=this->cost;
                improved = true;
                break;
            }

            for (int j = this->instance->n - 1; j > this->p; j--) {
                swapIndex(j, j - 1);
                calculateSolution();

                if (this->cost < bestCost) {
                    bestCost=this->cost;
                    improved = true;
                    break;
                }
            }
            this->p++;
            insert(this->p - 1, i);
            calculateSolution();
        }
    }

    improved=true;
    while(improved) {
        improved=false;
        for (int i = this->p; i < this->instance->n; i++) {

            this->p++;
            swapIndex(i, this->p - 1);
            calculateSolution();

            if (this->cost < bestCost) {
                bestCost=this->cost;
                improved = true;
                break;
            }

            for (int j = this->p - 1; j > 0; j--) {
                swapIndex(j, j - 1);
                calculateSolution();
                if (this->cost < bestCost) {
                    bestCost=this->cost;
                    improved = true;
                    break;
                }
            }
            this->p--;
            insert(0, i);
            calculateSolution();
        }
    }
}

//Complete ref local search
void Arena::blPc() {
    float bestCost=this->cost;
    for(int i=0;i<(this->instance->n - 1); i++){
        for(int j=i+1;j<this->instance->n; j++){
            swapIndex(i,j);
            calculateSolution();
            if(this->cost < bestCost){
                bestCost=this->cost;
                i=-1;
                break;
            }else{
                swapIndex(i,j);
                calculateSolution();
            }

        }
    }

}

//Return an index from a gived float value n, 0.0<= n <= 1.0
int Arena::giveMeIndex(float value) {
    return floor(this->instance->n * value);
}

//Swap two facilites
void Arena::swapIndex(int i, int j) {
    int aux=this->corridor[i];
    this->corridor[i]=this->corridor[j];
    this->corridor[j]=aux;
}



