//
// Created by rafael on 13/05/22.
//
#include "Problema.h"

void Problema::go() {
    construir();
    float melhorObjetivo=this->funcaoObjetivo;
    int melhorParticao=this->particao;
    int* melhorCorredor=new int[this->n];

    for(int i=0;i<this->n;i++){
        melhorCorredor[i]=this->corredorSolucao[i];
    }
    for(int i=0;i<100; i++){
        //begin cod

 rec(0.500000 ) ;
 bL4() ;
 bL4() ;
 bL1() ;
 repart( +0.480000 ) ;
 bL2() ;
 swap(0.780000 ,0.400000 ) ;
 RVND() ;
 RVND() ;
 swap(0.480000 ,0.850000 ) ;


        //end cod

        if(this->funcaoObjetivo<melhorObjetivo){
            melhorObjetivo=this->funcaoObjetivo;
            melhorParticao=this->particao;
            for(int j=0;j<this->n;j++){
                melhorCorredor[j]=this->corredorSolucao[j];
            }
        }else{
            this->funcaoObjetivo=melhorObjetivo;
            this->particao=melhorParticao;
            for(int j=0;j<this->n;j++){
                this->corredorSolucao[j]=melhorCorredor[j];
            }
        }
    }
    delete[] melhorCorredor;
}

Problema::Problema(int qtdSalas) {
    this->n=qtdSalas;
    this->particao=0;
    this->funcaoObjetivo=0;

    this->vetorDeSalas=nullptr;
    this->matrizDeTrafego=nullptr;
    this->corredorSolucao=new int[this->n];
    this->abcissas=new float[this->n];
}

Problema::~Problema() {
    for(int i=0;i<this->n; i++){
        Sala* sala=this->vetorDeSalas[i];
        delete sala;
    }
    delete[] this->vetorDeSalas;
    delete[] this->corredorSolucao;

    for(int i=0;i<this->n; i++){
        int* linha=this->matrizDeTrafego[i];
        delete[] linha;
    }
    delete []this->matrizDeTrafego;
}

void Problema::calcularSolucao() {
    montarAbcissas();
    calcularCusto();
}

void Problema::montarAbcissas() {
    int tamLado=0;

    int sala=-1;
    int comp=-1;

    for(int i=0;i<this->n;i++){
        if(i==this->particao){
            tamLado=0;
        }

        sala=this->corredorSolucao[i];
        comp=this->vetorDeSalas[sala]->getComprimento();
        this->abcissas[sala]=tamLado+(float)comp/2;
        tamLado+=comp;
    }
}

void Problema::calcularCusto() {
    this->funcaoObjetivo=0;

    for(int i=0; i < (this->n - 1); i++){
        int salaI=this->corredorSolucao[i];
        for(int j=i+1;j<this->n;j++){
            int salaJ=this->corredorSolucao[j];
            float distancia=abs(abcissas[salaJ]-abcissas[salaI]);
            float custoLocal=distancia*(float)matrizDeTrafego[salaI][salaJ];
            this->funcaoObjetivo+=custoLocal;
        }
    }
}
void Problema::construir() {
    for(int i=0;i<this->n;i++){
        this->corredorSolucao[i]=i;
    }
    this->particao=(this->n/2-2)+rand()%5;
    calcularSolucao();
    for(int i=0;i<this->n;i++){
        int j=rand()%this->n;
        int k=rand()%this->n;
        while(k==j){
            k=rand()%this->n;
        }
        swapIndex(i,j);
    }
}

void Problema::resetarProblema() {
    this->n=0;
    this->particao=0;
    this->funcaoObjetivo=0;

    delete[] this->corredorSolucao;
    this->corredorSolucao=new int[this->n];

    for(int i=0;i<this->n; i++){
        this->vetorDeSalas[i]->desalocar();
        this->abcissas[i]=0;
    }
}
void Problema::imprimirCorredor() {
    for(int i=0;i<this->n;i++){
        if(i==this->particao){
            cout<<endl;
        }
        cout<<this->corredorSolucao[i]<<" ";
    }
    cout<<endl;
}
//-------------operacoes---------------//

void Problema::swap(float iF, float jF) {
    int i= giveMeIndex(iF);
    int j= giveMeIndex(jF);
    if(i < this->n && j < this->n) {
        int aux = this->corredorSolucao[i];
        this->corredorSolucao[i] = this->corredorSolucao[j];
        this->corredorSolucao[j] = aux;
        calcularSolucao();
    }
}
void Problema::repart(float fatorF) {
    int fator= giveMeIndex(fatorF);
    if((this->particao + fator) < this->n && (this->particao + fator) > 0) {
        this->particao += fator;
        calcularSolucao();
    }
}
void Problema::shake(float tamF) {

    int tam= giveMeIndex(tamF);
    if(tam < this->n){

        for(int i=0; i < tam; i++){
            int j=rand()%this->n;
            int k=rand()%this->n;
            while(k==j){
                k=rand()%this->n;
            }
        }

        calcularSolucao();
    }
}
void Problema::rec(float tamF){
    int tam= giveMeIndex(tamF);

    if(tam<this->n) {
        int *desconstruidas = new int[tam];

        for (int i = 0; i < tam; i++) {
            int j = rand() % this->n;
            desconstruidas[i] = this->corredorSolucao[j];

            this->n--;

            for (int k = j; k < this->n; k++) {
                this->corredorSolucao[k] = this->corredorSolucao[k + 1];
            }

            if (j < this->particao) {
                this->particao--;
            }
            calcularSolucao();
        }

        for (int i = 0; i < tam; i++) {
            this->corredorSolucao[this->n] = desconstruidas[i];
            this->n++;
        }

        calcularSolucao();
        delete[] desconstruidas;
    }
}
void Problema::RVND() {
    bool melhorado=true;

    int vetRand[4]={0,1,2,3};
    int fase=0;
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

void Problema::bLFase1(bool *melhorado) {
    float melhorSolucao=this->funcaoObjetivo;

    for(int i=1;i<this->particao;i++){
        swap(i-1,i);

        if(this->funcaoObjetivo<melhorSolucao){
            *melhorado=true;
            return;
        }else{
            swap(i-1,i);
            this->funcaoObjetivo=melhorSolucao;
        }
    }
    for(int i=this->particao+1;i<this->n;i++){
        swap(i-1,i);

        if(this->funcaoObjetivo<melhorSolucao){
            *melhorado=true;
            return;
        }else{
            swap(i-1,i);
            this->funcaoObjetivo=melhorSolucao;
        }
    }
}

void Problema::bLFase2(bool *melhorado) {
    float melhorSolucao=this->funcaoObjetivo;

    for(int i=0;i<(this->particao-2);i++){

        for(int j=i+2;j<this->particao;j++) {
            swap(i , j);

            if (this->funcaoObjetivo < melhorSolucao) {
                *melhorado = true;
                return;
            } else {
                swap(i , j);
                this->funcaoObjetivo = melhorSolucao;
            }
        }
    }

    for(int i=this->particao;i<(this->n-2);i++){

        for(int j=i+2;j<this->n;j++) {
            swap(i , j);

            if (this->funcaoObjetivo < melhorSolucao) {
                *melhorado = true;
                return;
            } else {
                swap(i , j);
                this->funcaoObjetivo = melhorSolucao;
            }
        }
    }
}

void Problema::bLFase3(bool *melhorado) {
    float melhorSolucao=this->funcaoObjetivo;
    for(int i=0;i<this->particao;i++){
        for(int j=this->particao;j<this->n;j++){
            swap(i, j);

            if (this->funcaoObjetivo < melhorSolucao) {
                *melhorado = true;
                return;
            } else {
                swap(i, j);
                this->funcaoObjetivo=melhorSolucao;
            }
        }
    }
}

void Problema::bLFase4(bool *melhorado) {
    float melhorSolucao=this->funcaoObjetivo;

    for (int i = 0; i < this->particao; i++) {
        int salaI = this->corredorSolucao[i];

        this->particao--;
        insert(i, this->n-1);

        if (this->funcaoObjetivo < melhorSolucao) {
            *melhorado = true;
            return;
        }

        for (int j = this->n-1; j > this->particao; j--) {
            swap(j, j-1);

            if (this->funcaoObjetivo < melhorSolucao) {
                *melhorado = true;
                return;
            }
        }
        this->particao++;
        insert(this->particao - 1, i);
    }



    for (int i = this->particao; i < this->n; i++) {
        int salaI = this->corredorSolucao[i];
        int comprimentoI = this->vetorDeSalas[salaI]->getComprimento();

        // if ((this->comprimentoD - comprimentoI) >= this->comprimentoE) {
        this->particao++;
        swap(i, this->particao - 1);

        if (this->funcaoObjetivo < melhorSolucao) {
            *melhorado = true;
            return;
        }

        for (int j = this->particao - 1; j > 0; j--) {
            swap(j, j - 1);
            if (this->funcaoObjetivo < melhorSolucao) {
                *melhorado = true;
                return;
            }
        }
        this->particao--;
        insert(0, i);
    }

}

void Problema::insert(int iOrigem, int iDestino) {
    if(iOrigem<this->n && iOrigem>=0 && iDestino<this->n && iDestino>=0) {

        int aux = this->corredorSolucao[iOrigem];
        if (iOrigem < iDestino) {

            for (int j = iOrigem; j < iDestino; j++) {
                this->corredorSolucao[j] = this->corredorSolucao[j + 1];
            }

        } else {

            for (int j = iOrigem; j > iDestino; j--) {
                this->corredorSolucao[j] = this->corredorSolucao[j - 1];
            }

        }

        this->corredorSolucao[iDestino] = aux;
        calcularSolucao();
    }
}

void Problema::bL1() {
    float melhorSolucao=this->funcaoObjetivo;

    for(int i=1;i<this->particao;i++){
        swap(i-1,i);

        if(this->funcaoObjetivo<melhorSolucao){
            melhorSolucao= this->funcaoObjetivo;
            i=0;
        }else{
            swap(i-1,i);
        }
    }
    for(int i=this->particao+1;i<this->n;i++){
        swap(i-1,i);

        if(this->funcaoObjetivo<melhorSolucao){
            melhorSolucao= this->funcaoObjetivo;
            i=this->particao;
        }else{
            swap(i-1,i);
        }
    }
}

void Problema::bL2() {
    float melhorSolucao=this->funcaoObjetivo;

    for(int i=0;i<(this->particao-2);i++){

        for(int j=i+2;j<this->particao;j++) {
            swap(i , j);

            if (this->funcaoObjetivo < melhorSolucao) {
                melhorSolucao= this->funcaoObjetivo;
                i=-1;
                break;
            } else {
                swap(i , j);
            }
        }
    }

    for(int i=this->particao;i<(this->n-2);i++){

        for(int j=i+2;j<this->n;j++) {
            swap(i , j);

            if (this->funcaoObjetivo < melhorSolucao) {
                melhorSolucao= this->funcaoObjetivo;
                i=this->particao-1;
                break;
            } else {
                swap(i , j);
                this->funcaoObjetivo = melhorSolucao;
            }
        }
    }
}

void Problema::bL3() {
    float melhorSolucao=this->funcaoObjetivo;

    for (int i = 0; i < this->particao; i++) {
        for (int j = this->particao; j < this->n; j++) {
            swap(i, j);

            if (this->funcaoObjetivo < melhorSolucao) {
                melhorSolucao=this->funcaoObjetivo;
                i=-1;
                break;
            } else {
                swap(i, j);
            }
        }
    }


}

void Problema::bL4() {
    float melhorSolucao=this->funcaoObjetivo;

    bool melhorado=true;

    while(melhorado) {
        melhorado = false;
        for (int i = 0; i < this->particao; i++) {
            int salaI = this->corredorSolucao[i];

            this->particao--;
            insert(i, this->n - 1);

            if (this->funcaoObjetivo < melhorSolucao) {
                melhorSolucao=this->funcaoObjetivo;
                melhorado = true;
                break;
            }

            for (int j = this->n - 1; j > this->particao; j--) {
                swap(j, j - 1);

                if (this->funcaoObjetivo < melhorSolucao) {
                    melhorSolucao=this->funcaoObjetivo;
                    melhorado = true;
                    break;
                }
            }
            this->particao++;
            insert(this->particao - 1, i);
        }
    }

    melhorado=true;
    while(melhorado) {
        melhorado=false;
        for (int i = this->particao; i < this->n; i++) {
            int salaI = this->corredorSolucao[i];

            this->particao++;
            swap(i, this->particao - 1);

            if (this->funcaoObjetivo < melhorSolucao) {
                melhorSolucao=this->funcaoObjetivo;
                melhorado = true;
                break;
            }

            for (int j = this->particao - 1; j > 0; j--) {
                swap(j, j - 1);
                if (this->funcaoObjetivo < melhorSolucao) {
                    melhorSolucao=this->funcaoObjetivo;
                    melhorado = true;
                    break;
                }
            }
            this->particao--;
            insert(0, i);
        }
    }
}

void Problema::blPc() {
    float melhorSol=this->funcaoObjetivo;
    for(int i=0;i<(this->n-1);i++){
        for(int j=i+1;j<this->n;j++){
            swap(i,j);

            if(this->funcaoObjetivo<melhorSol){
                melhorSol=this->funcaoObjetivo;
                i=-1;
                break;
            }

        }
    }

}

int Problema::giveMeIndex(float value) {
    return floor(this->n*value);
}

void Problema::swapIndex(int i, int j) {
    int aux=this->corredorSolucao[i];
    this->corredorSolucao[i]=this->corredorSolucao[j];
    this->corredorSolucao[j]=aux;
}
