//
// Created by rafael on 13/05/22.
//

#include "Instancia.h"

Instancia::Instancia() {
    this->nome="";
    this->litSol=-1.0;
    this->n=-1;
    this->funcaoObjetivo=-1;
    this->particao=-1;
    this->comprimentos= nullptr;
    this->demandas = nullptr;
    this->corredor= nullptr;
    this->abcissas= nullptr;
}

Instancia::~Instancia() {

    delete[] this->comprimentos;
    delete[] this->corredor;
    delete[] this->abcissas;
    for(int i=0;i<n;i++){
        delete[] this->demandas[i];
    }
    delete[] this->demandas;
}

//Aloca o vetor de comprimentos
void Instancia::alocarComprimentos() {
    if(this->n!=-1){
        this->comprimentos=new int[this->n];
    }else{
        cout<<"n Não conhecido"<<endl;
        exit(1);
    }
}

//Aloca matriz de demandas
void Instancia::alocarDemandas() {
    if(this->n!=-1){
        this->demandas=new int*[this->n];
        for(int i=0;i<this->n;i++){
            this->demandas[i]=new int[this->n];
        }
    }else{
        cout<<"n Não conhecido"<<endl;
        exit(1);
    }
}

void Instancia::gerarSolucaoInicial() {

    if(this->n!=-1){
        if (this->corredor == nullptr) {
            this->corredor = new int[this->n];
            this->abcissas = new int[this->n];
        }
    }else{
        cout<<"n Não conhecido"<<endl;
        exit(1);
    }

    for(int i=0;i<this->n;i++){
        this->corredor[i]=i;
    }
    this->particao=(this->n/2-2)+rand()%5;
    for(int i=0;i<this->n;i++){
        int j=rand()%this->n;
        int k=rand()%this->n;
        while(k==j){
            k=rand()%this->n;
        }
        swapIndex(i,j);
    }
    calcularSolucao();
}

void Instancia::imprimirComprimentos() {
    if(this->n!=-1) {
        for (int i = 0; i <this->n;i++){
            cout<<this->comprimentos[i]<<" ";
        }
        cout<<endl;
    }else{
        cout<<"n Não conhecido"<<endl;
        exit(1);
    }
}

void Instancia::imprimirDemandas() {
    if(this->n!=-1) {
        for (int i = 0; i <this->n;i++){
            for(int j=0;j<this->n;j++){
                cout<<this->demandas[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }else{
        cout<<"n Não conhecido"<<endl;
        exit(1);
    }
}

void Instancia::imprimirLayout() {
    if(this->n!=-1) {
        for (int i = 0; i <this->n;i++){
            cout << this->corredor[i] << " ";
        }
        cout<<endl;
    }else{
        cout<<"n Não conhecido"<<endl;
        exit(1);
    }
}

void Instancia::swapIndex(int i, int j) {
    int aux=this->corredor[i];
    this->corredor[i]=this->corredor[j];
    this->corredor[j]=aux;
}

void Instancia::calcularSolucao() {
    //Calculando Abcissa
    int tamLado=0;

    int sala=-1;
    int comp=-1;

    for(int i=0;i<this->n;i++){
        if(i==this->particao){
            tamLado=0;
        }

        sala=this->corredor[i];
        comp=this->comprimentos[sala];
        this->abcissas[sala]=tamLado+(float)comp/2;
        tamLado+=comp;
    }

    //Calculando Custo
    this->funcaoObjetivo=0;

    for(int i=0; i < (this->n - 1); i++){
        int salaI=this->corredor[i];
        for(int j=i+1;j<this->n;j++){
            int salaJ=this->corredor[j];
            float distancia=abs(abcissas[salaJ]-abcissas[salaI]);
            float custoLocal=distancia*(float)this->demandas[salaI][salaJ];
            this->funcaoObjetivo+=custoLocal;
        }
    }
}






