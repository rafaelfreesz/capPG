//
// Created by rafael on 08/06/22.
//
#include "Arena.h"

Arena::Arena(Instancia *instancia) {

    this->instancia=instancia;
    this->p=0;
    this->fo=0;
    this->corredor=new int[this->instancia->n];
    this->abcissas=new float[this->instancia->n];
    this->n=this->instancia->n;
}



Arena::~Arena() {
    delete[] this->corredor;
    delete[] this->abcissas;
}

void Arena::go(vector<string>* individuoTokenizado) {

    importarSolucao();

    float melhorObjetivo=this->fo;
    int melhorParticao=this->p;
    int* melhorCorredor=new int[this->instancia->n];

    for(int i=0;i<50;i++) {

        interpretar(individuoTokenizado);

        if(this->fo < melhorObjetivo){
            melhorObjetivo=this->fo;
            melhorParticao=this->p;
            for(int j=0;j<this->instancia->n;j++){
                melhorCorredor[j]=this->corredor[j];
            }
        }
    }

    this->fo=melhorObjetivo;
    this->p=melhorParticao;
    for(int i=0;i<this->instancia->n;i++){
        this->corredor[i]=melhorCorredor[i];
    }
    delete[] melhorCorredor;
}

void Arena::interpretar(vector<string> *individuoLinear) {
    int i = 0;
    while (i < individuoLinear->size()) {

        if (individuoLinear->at(i) == "swap(") {
            swap(stof(individuoLinear->at(i + 1)), stof(individuoLinear->at(i + 3)));
            i += 5;
        } else if (individuoLinear->at(i) == "repart(") {
            string sentence = individuoLinear->at(i + 1) + individuoLinear->at(i + 2);
            repart(stof(sentence));
            i += 4;
        } else if (individuoLinear->at(i) == "shake(") {
            shake(stof(individuoLinear->at(i + 1)));
            i += 3;
        } else if (individuoLinear->at(i) == "rec(") {
            rec(stof(individuoLinear->at(i + 1)));
            i += 3;
        } else if (individuoLinear->at(i) == "RVND();") {
            RVND();
            i++;
        } else if (individuoLinear->at(i) == "bL1();") {
            bL1();
            i++;
        } else if (individuoLinear->at(i) == "bL2();") {
            bL2();
            i++;
        } else if (individuoLinear->at(i) == "bL3();") {
            bL3();
            i++;
        } else if (individuoLinear->at(i) == "bL4();") {
            bL4();
            i++;
        } else {
            blPc();
            i++;
        }
    }

    if (i > individuoLinear->size()) {
        cout << "Erro de casamento de padrão" << endl;
        exit(2);
    }

}
void Arena::importarSolucao() {

    for(int i=0;i<this->instancia->n;i++){
        this->corredor[i]=this->instancia->corredor[i];
        this->abcissas[i]=this->instancia->abcissas[i];
    }
    this->p=this->instancia->particao;
    this->fo=this->instancia->funcaoObjetivo;

}
void Arena::montarAbcissas() {
    int tamLado=0;

    int sala=-1;
    int comp=-1;

    for(int i=0;i<this->n;i++){
        if(i==this->p){
            tamLado=0;
        }

        sala=this->corredor[i];
        comp=this->instancia->comprimentos[sala];
        this->abcissas[sala]=tamLado+(float)comp/2;
        tamLado+=comp;
    }
}
void Arena::calcularCusto() {
    this->fo=0;

    for(int i=0; i < (this->n - 1); i++){
        int salaI=this->corredor[i];
        for(int j=i+1;j<this->n;j++){
            int salaJ=this->corredor[j];
            float distancia=abs(abcissas[salaJ]-abcissas[salaI]);
            float custoLocal=distancia*(float)this->instancia->demandas[salaI][salaJ];
            this->fo+=custoLocal;
        }
    }
}
void Arena::calcularSolucao() {
    montarAbcissas();
    calcularCusto();
}
void Arena::imprimirCorredor() {
    cout<<endl;
    for(int i=0;i<this->n;i++){
        if(i==this->p){
            cout<<endl;
        }
        cout << this->corredor[i] << " ";
    }
    cout<<endl;
}

void Arena::swap(float iF, float jF) {
    int i= giveMeIndex(iF);
    int j= giveMeIndex(jF);
    if(i < this->instancia->n && j < this->instancia->n) {
        int aux = this->corredor[i];
        this->corredor[i] = this->corredor[j];
        this->corredor[j] = aux;
    }
    calcularSolucao();
}

//-------------operacoes---------------//

void Arena::repart(float fatorF) {
    int fator= giveMeIndex(fatorF);
    if((this->p + fator) < this->instancia->n && (this->p + fator) > 0) {
        this->p += fator;
        calcularSolucao();
    }
}

void Arena::shake(float tamF) {

    int tam= giveMeIndex(tamF);
    if(tam > 0){

        for(int i=0; i < tam; i++){
            int j=rand()%this->instancia->n;
            int k=rand()%this->instancia->n;
            while(k==j){
                k=rand()%this->instancia->n;
            }
            swapIndex(j,k);
        }
        calcularSolucao();
    }
}
void Arena::rec(float tamF){
    int tam= giveMeIndex(tamF);

    if(tam<this->instancia->n) {
        int* desconstruidas= new int [tam];

        
        for (int i = 0; i < tam; i++) {
            int j = rand() % this->n;
            desconstruidas[i] = corredor[j];

            this->n--;

            for (int k = j; k < this->n; k++) {
                this->corredor[k] = this->corredor[k + 1];
            }

            if (j < this->p) {
                this->p--;
            }
        }
        bool esq=((this->p) < (this->n - this->p));
        for (int i = 0; i < tam; i++) {

            this->corredor[this->n] = desconstruidas[i];
            if(esq){
                for(int j=this->n-1;j>=this->p;j--){
                    swapIndex(j,j+1);
                }
                this->p++;
            }
            esq=!esq;
            this->n++;
        }


        calcularSolucao();
        delete[] desconstruidas;
    }
}
void Arena::RVND() {
    bool melhorado=true;

    int vetRand[4]={0,1,2,3};
    int fase=0;
    int u=0;
    while(fase < 4){
        melhorado=false;

        if(fase==0){
            for(int i=0;i<4;i++){
                int j=rand()%4;
                int k=rand()%4;
                while(k==j){
                    k=rand()%4;
                }
                int aux=vetRand[j];
                vetRand[j]=vetRand[k];
                vetRand[k]=aux;
            }
        }

        switch (vetRand[fase]) {
            case 0: bLFase1(&melhorado);
                if(melhorado){
                    fase=0;
                }else{
                    fase++;
                }
                break;
            case 1: bLFase2(&melhorado);
                if(melhorado){
                    fase=0;
                }else{
                    fase++;
                }
                break;
            case 2: bLFase3(&melhorado);
                if(melhorado){
                    fase=0;
                }else{
                    fase++;
                }
                break;
            case 3: bLFase4(&melhorado);
                if(melhorado){
                    fase=0;
                }else{
                    fase++;
                }
                break;
        }

    }
}

void Arena::bLFase1(bool *melhorado) {
    float melhorSolucao=this->fo;

    for(int i=1;i<this->p; i++){
        swapIndex(i-1,i);
        calcularSolucao();

        if(this->fo < melhorSolucao){
            *melhorado=true;
            return;
        }else{
            swapIndex(i-1,i);
            calcularSolucao();
        }
    }
    for(int i= this->p + 1; i < this->instancia->n; i++){
        swapIndex(i-1,i);
        calcularSolucao();

        if(this->fo < melhorSolucao){
            *melhorado=true;
            return;
        }else{
            swapIndex(i-1,i);
            calcularSolucao();
        }
    }
}

void Arena::bLFase2(bool *melhorado) {
    float melhorSolucao=this->fo;

    for(int i=0;i<(this->p - 2); i++){

        for(int j=i+2;j<this->p; j++) {
            swapIndex(i , j);
            calcularSolucao();

            if (this->fo < melhorSolucao) {
                *melhorado = true;
                return;
            } else {
                swapIndex(i , j);
                calcularSolucao();
            }
        }
    }

    for(int i=this->p; i < (this->instancia->n - 2); i++){

        for(int j=i+2;j<this->instancia->n;j++) {
            swapIndex(i , j);
            calcularSolucao();

            if (this->fo < melhorSolucao) {
                *melhorado = true;
                return;
            } else {
                swapIndex(i , j);
                calcularSolucao();
            }
        }
    }
}

void Arena::bLFase3(bool *melhorado) {
    float melhorSolucao=this->fo;
    for(int i=0;i<this->p; i++){
        for(int j=this->p; j < this->instancia->n; j++){
            swapIndex(i, j);
            calcularSolucao();

            if (this->fo < melhorSolucao) {
                *melhorado = true;
                return;
            } else {
                swapIndex(i, j);
                calcularSolucao();
            }
        }
    }
}

void Arena::bLFase4(bool *melhorado) {
    float melhorSolucao=this->fo;

    for (int i = 0; i < this->p; i++) {

        this->p--;
        insert(i, this->instancia->n-1);
        calcularSolucao();

        if (this->fo < melhorSolucao) {
            *melhorado = true;
            return;
        }

        for (int j = this->instancia->n-1; j > this->p; j--) {
            swapIndex(j, j-1);
            calcularSolucao();

            if (this->fo < melhorSolucao) {
                *melhorado = true;
                return;
            }
        }
        this->p++;
        insert(this->p - 1, i);
        calcularSolucao();
    }



    for (int i = this->p; i < this->instancia->n; i++) {

        this->p++;
        insert(i, this->p - 1);
        calcularSolucao();

        if (this->fo < melhorSolucao) {
            *melhorado = true;
            return;
        }

        for (int j = this->p - 1; j > 0; j--) {
            swapIndex(j, j - 1);
            calcularSolucao();
            if (this->fo < melhorSolucao) {
                *melhorado = true;
                return;
            }
        }
        this->p--;
        insert(0, i);
        calcularSolucao();
    }

}

void Arena::insert(int iOrigem, int iDestino) {
    if(iOrigem<this->instancia->n && iOrigem>=0 && iDestino<this->instancia->n && iDestino>=0 && iOrigem!=iDestino) {

        int aux = this->corredor[iOrigem];
        if (iOrigem < iDestino) {

            for (int j = iOrigem; j < iDestino; j++) {
                this->corredor[j] = this->corredor[j + 1];
            }

        } else {

            for (int j = iOrigem; j > iDestino; j--) {
                this->corredor[j] = this->corredor[j - 1];
            }

        }

        this->corredor[iDestino] = aux;
    }
}

void Arena::bL1() {
    float melhorSolucao=this->fo;

    for(int i=1;i<this->p; i++){
        swapIndex(i-1,i);
        calcularSolucao();

        if(this->fo < melhorSolucao){
            melhorSolucao= this->fo;
            i=0;
        }else{
            swapIndex(i-1,i);
            calcularSolucao();
        }
    }
    for(int i= this->p + 1; i < this->instancia->n; i++){
        swapIndex(i-1,i);
        calcularSolucao();

        if(this->fo < melhorSolucao){
            melhorSolucao= this->fo;
            i=this->p;
        }else{
            swapIndex(i-1,i);
            calcularSolucao();
        }
    }
}

void Arena::bL2() {
    float melhorSolucao=this->fo;

    for(int i=0;i<(this->p - 2); i++){

        for(int j=i+2;j<this->p; j++) {
            swapIndex(i , j);
            calcularSolucao();

            if (this->fo < melhorSolucao) {
                melhorSolucao= this->fo;
                i=-1;
                break;
            } else {
                swapIndex(i , j);
                calcularSolucao();
            }
        }
    }

    for(int i=this->p; i < (this->instancia->n - 2); i++){

        for(int j=i+2;j<this->instancia->n;j++) {
            swapIndex(i , j);
            calcularSolucao();

            if (this->fo < melhorSolucao) {
                melhorSolucao= this->fo;
                i= this->p - 1;
                break;
            } else {
                swapIndex(i , j);
                calcularSolucao();
            }
        }
    }
}

void Arena::bL3() {
    float melhorSolucao=this->fo;

    for (int i = 0; i < this->p; i++) {
        for (int j = this->p; j < this->instancia->n; j++) {
            swapIndex(i, j);
            calcularSolucao();

            if (this->fo < melhorSolucao) {
                melhorSolucao=this->fo;
                i=-1;
                break;
            } else {
                swapIndex(i, j);
                calcularSolucao();
            }
        }
    }


}

void Arena::bL4() {
    float melhorSolucao=this->fo;

    bool melhorado=true;

    while(melhorado) {
        melhorado = false;
        for (int i = 0; i < this->p; i++) {

            this->p--;
            insert(i, this->instancia->n - 1);
            calcularSolucao();

            if (this->fo < melhorSolucao) {
                melhorSolucao=this->fo;
                melhorado = true;
                break;
            }

            for (int j = this->instancia->n - 1; j > this->p; j--) {
                swapIndex(j, j - 1);
                calcularSolucao();

                if (this->fo < melhorSolucao) {
                    melhorSolucao=this->fo;
                    melhorado = true;
                    break;
                }
            }
            this->p++;
            insert(this->p - 1, i);
            calcularSolucao();
        }
    }

    melhorado=true;
    while(melhorado) {
        melhorado=false;
        for (int i = this->p; i < this->instancia->n; i++) {

            this->p++;
            swapIndex(i, this->p - 1);
            calcularSolucao();

            if (this->fo < melhorSolucao) {
                melhorSolucao=this->fo;
                melhorado = true;
                break;
            }

            for (int j = this->p - 1; j > 0; j--) {
                swapIndex(j, j - 1);
                calcularSolucao();
                if (this->fo < melhorSolucao) {
                    melhorSolucao=this->fo;
                    melhorado = true;
                    break;
                }
            }
            this->p--;
            insert(0, i);
            calcularSolucao();
        }
    }
}

void Arena::blPc() {
    float melhorSol=this->fo;
    for(int i=0;i<(this->instancia->n-1);i++){
        for(int j=i+1;j<this->instancia->n;j++){
            swapIndex(i,j);
            calcularSolucao();
            if(this->fo < melhorSol){
                melhorSol=this->fo;
                i=-1;
                break;
            }else{
                swapIndex(i,j);
                calcularSolucao();
            }

        }
    }

}

int Arena::giveMeIndex(float value) {
    return floor(this->instancia->n*value);
}

void Arena::swapIndex(int i, int j) {
    int aux=this->corredor[i];
    this->corredor[i]=this->corredor[j];
    this->corredor[j]=aux;
}



