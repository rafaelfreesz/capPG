//
// Created by rafael on 13/05/22.
//

#include "Entrada.h"

Entrada::Entrada(string arquivo){
    this->arquivo=arquivo;
    this->file=NULL;
    abrirArquivo();
}

Entrada::~Entrada(){
    fecharArquivo();
}

//Metodo responsavel por abrir arquivo de entrada
void Entrada::abrirArquivo() {
    if(this->file==NULL) {
        this->file = fopen(stringToChar(this->arquivo), "r");
        if (this->file == NULL) {
            cout << "Erro ao abrir o arquivo" << endl;
        }
    }else{
        cout<<"O arquivo ja esta aberto"<<endl;
    }
}

//Metodo responsavel por fechar arquivo de entrada
void Entrada::fecharArquivo() {
    if (this->file!=NULL){
        fclose(this->file);
    }
}

//Funcao que retorna proxima linha de string do arquivo de entrada
char* Entrada::proximaLinha() {
    char *cadeia=new char[256];


    if(fgets(cadeia,256,this->file)!=NULL) {
        for (int i = 0; i < 256 && cadeia[i] != '\n'; i++) {
            if (cadeia[i] == ' '|| cadeia[i]=='\t') {
                cadeia[i] = ',';
            }

        }
        return cadeia;
    }else{
        delete[]cadeia;
        return NULL;
    }

}

//Retorna o nome da proxima instancia
string Entrada::proximaInstancia(){
    char *cadeia=new char[256];
    string strRetorno="";
    while(fgets(cadeia,256,this->file)!=NULL) {
        if(cadeia[0]!='/' && cadeia[1]!='/') {
            for (int i = 0; i < 256 && cadeia[i] != '\n'; i++) {
                if (cadeia[i] == ' ' || cadeia[i] == '\t') {
                    cadeia[i] = ',';
                }

            }
            strRetorno=charToString(cadeia);
            break;

        }
    }
    delete[]cadeia;
    return strRetorno;

}

//Funcao que converte string para cadeia de caracteres e o retorna
char* Entrada::stringToChar(string linha){
    char* cadeia= new char[linha.size()+1];

    for(int i=0;i<linha.size();i++){
        cadeia[i]=linha[i];
    }
    cadeia[linha.size()]='\0';

    return cadeia;
}

//Funcao que converte cadeia de caracteres para string e o retorna
string Entrada::charToString(char * linha) {
    string texto="";
    if(linha!=NULL) {
        for (int i = 0; linha[i] != '\n' && linha[i] != '\0' && linha[i] != '\r'; i++) {
            texto += linha[i];
        }
    }
    return texto;

}

//Funcao que monta uma instancia do Problema com base no arquivo de entrada
Problema* Entrada::montarProblema() {
    char* linha=proximaLinha();
    Problema* problema=new Problema(stol(linha));
    delete[] linha;

    problema->vetorDeSalas= montarVetorDeSalas(problema->n);
    problema->matrizDeTrafego= montarMatrizDeTrafego(problema->n);
    return problema;
}

//Funcao que monta e retorna o vetor de salas com os parametros de comprimento, vindos da primeira linha arquivo de entrada
Sala** Entrada::montarVetorDeSalas(int qtdSalas) {
    Sala** salas=new Sala*[qtdSalas];
    char* linha=proximaLinha();
    char*token=strtok(linha,",");
    for(int i=0;i<qtdSalas;i++){
        salas[i]=new Sala();
        salas[i]->setId(i);
        int comp=stol(token);
        salas[i]->setComprimento(comp);
        token=strtok(NULL,",");
    }
    delete[] linha;
    return salas;
}

//Funcao que monta e retorna a matriz de media de trafego entra as salas i e j
int** Entrada::montarMatrizDeTrafego(int qtdSalas) {
    int** trafegos=new int*[qtdSalas];
    char* linha;
    char*token;
    for(int i=0;i<qtdSalas;i++){
        linha=proximaLinha();

        if(linha[0]=='\r' && linha[1]=='\n'){
            delete[] linha;
            linha=proximaLinha();
        }

        token=strtok(linha,",");
        trafegos[i]=new int[qtdSalas];
        for(int j=0;j<qtdSalas;j++) {
            trafegos[i][j] = stol(token);
            token = strtok(NULL, ",");
        }
        delete[] linha;
    }
    return trafegos;
}