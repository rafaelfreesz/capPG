//
// Created by rafael on 22/02/2022.
//

#include "Search.h"

Search::Search(Configuracoes *conf, Gramatica *grammar, int seed) {
    this->conf=conf;
    this->grammar=grammar;
    this->seed=seed;
    montarInstancias();

}

Search::~Search() {
    delete this->conf;
}

void Search::montarInstancias() {

    Instancia* instancia;
    ifstream fileStream;
    string linha;

    //Importando o nome da instancia e a solução da literatura
    linha="./Instancias/lit.sol";
    fileStream.open(linha.c_str());
    while(getline(fileStream, linha)){

        this->instancias.emplace_back();
        instancia=&this->instancias.at(this->instancias.size()-1);


        vector<string> tokens;
        tokenize(linha, tokens, ";");

        instancia->nome=tokens.at(0);
        instancia->litSol=stof(tokens.at(1));

    }
    fileStream.close();

    //Importando os dados da instancia
    for(int i=0;i<this->instancias.size();i++){
        instancia=&this->instancias.at(i);
        linha="./Instancias/"+instancia->nome;

        fileStream.open(linha.c_str());

        getline(fileStream,linha);
        instancia->n= stoi(linha);

        //Importando comprimentos
        instancia->alocarComprimentos();
        getline(fileStream,linha);

        vector<string> comprTokens;
        tokenize(linha, comprTokens, " ");

        for(int j=0;j<instancia->n;j++){
            instancia->comprimentos[j]= stoi(comprTokens.at(j));
        }

        //Importando demandas
        instancia->alocarDemandas();

        for(int j=0;j<instancia->n;j++){
            getline(fileStream,linha);
            vector<string> demTokens;
            tokenize(linha, demTokens, " ");
            for(int k=0;k<instancia->n;k++){
                instancia->demandas[j][k]=stoi(demTokens[k]);
            }
        }

        fileStream.close();
    }
}

void Search::inciar() {

    Saida* relatorioGeracoes=new Saida("Resultados");
    relatorioGeracoes->escreverLinha("Semente de execução: "+ to_string(this->seed));

    cout<<"-------Criando população inicial"<<endl;

    this->criarPopulacaoInicial();
    avaliarPopulacao(0, this->conf->tamPop, 0);
    stable_sort(this->populacao, this->populacao + conf->tamPop, sortPopulationFitness);

    cout<<"--------População Inicial Criada."<<endl;

    relatorioGeracoes->imprimirResultado(this->populacao, this->conf->tamPop, 0, this->conf->sementeGeracao);

    double tempoGeracao;
    cout<<"Iniciando gerações:"<<endl;
    for(int it=1; it<this->conf->geracoes; it++){
        clock_t time=clock();
        this->conf->sementeGeracao=clock();
        cout << "--------Geração " + to_string(it) + ", Semente: " + to_string(this->conf->sementeGeracao) + " :" << endl;

        operate();

        avaliarPopulacao(this->conf->tamPop, this->conf->tamPop * 2, it);

        stable_sort(this->populacao + this->conf->tamPop, this->populacao + this-> conf->tamPop * 2, sortPopulationFitness);


        replace();
        stable_sort(this->populacao, this->populacao + this->conf->tamPop * 2, sortPopulationFitness);

        for(int i = this->conf->tamPop; i < this->conf->tamPop * 2; i++){
            delete this->populacao[i];
        }

        time=clock()-time;
        cout<<"--------Fim da Geração "+to_string(it)+" em "+ to_string((double)time/CLOCKS_PER_SEC)+" s."<<endl<<endl;
        relatorioGeracoes->imprimirResultado(this->populacao, this->conf->tamPop, it, this->conf->sementeGeracao);
    }
}

void Search::replace() {
    for(int i = this->conf->tamPop * this->conf->elitismo, j = this->conf->tamPop; i < this->conf->tamPop; i++, j++) {
        swap(this->populacao[i], this->populacao[j]);
    }
}
//Realização de cruamemento e mutação
void Search::operate() {
    int numIndividuos=this->conf->numParentais;
    Individuo ** selecionados;

    for(int i=this->conf->tamPop; i < this->conf->tamPop * 2; i+=numIndividuos){


        selecionados = new Individuo *[numIndividuos];
        randPais(selecionados, i); //Selecionado pais

        crossover(selecionados);

        mutate(selecionados);

        //Limpando os ponteiros
        for(int j = 0; j < numIndividuos; j++){
            selecionados[j] = NULL;
        }
        delete selecionados;

    }


}
//Operação de mutação
void Search::mutate(Individuo **selecionados) {
    for(int i=0;i<conf->numParentais; i++){
        if(rand()%101< this->conf->taxaMutacao * 100){
            auxMutate(selecionados[i]->tree);
        }
    }

}
void Search::auxMutate(Arvore *t) {
    No* n = t->subTree();
    n->erase();
    this->grammar->derivate(n);
    t->update();
}
//Operação de crossover
void Search::crossover(Individuo **selecionados) {
    int numIndividuos=this->conf->numParentais;

    for(int i=1;i<numIndividuos;i+=2){
        if(rand()%101< this->conf->taxaCrossover * 100){
            treeCrossover(selecionados[i-1]->tree,selecionados[i]->tree);

        }
    }

}

void Search::treeCrossover(Arvore *a, Arvore *b) {
    No* n = a->subTree();
    No* m = b->targetSubTree(n);

    if(n == NULL || m == NULL) {
        return;
    }
    swap(n->sons, m->sons);
    swap(n->father, m->father);

    a->update();
    b->update();
}

//Sorteia os pais
void Search::randPais(Individuo **pais, int ind) {

    for(int i=0;i<this->conf->numParentais; i+=2) {
        int x = rand() % this->conf->tamPop;
        int y = rand() % this->conf->tamPop;

        while (x == y) {
            y = rand() % this->conf->tamPop;
        }

        pais[i]=this->populacao[x]->clone();
        pais[i+1]=this->populacao[y]->clone();

        this->populacao[ind]=pais[i];
        this->populacao[ind + 1]=pais[i + 1];


    }

}
void Search::gerarSolucoesIniciais() {
    for(int i=0;i<this->instancias.size();i++){
        this->instancias[i].gerarSolucaoInicial();
    }
}
void Search::exportarResultadoIndividuo(int index) {
    string linha="r_"+this->populacao[index]->nome;
    ofstream agent("Population/Resultados/" + linha);
    for(int i=0;i<this->instancias.size();i++){
        linha= to_string(this->populacao[index]->solucoes[i])+"; "+ to_string(this->populacao[index]->tempos[i]);
        agent << linha<<endl;
    }
    agent.close();
}
void Search::exportarIndividuo(int index) {
    string linha=this->populacao[index]->nome;
    ofstream agent("Population/" + linha);

    linha= this->populacao[index]->tree->str();
    vector<string> terminais;
    tokenize(linha,terminais,";");

    for(int i=0;i<terminais.size();i++){
        agent << terminais.at(i)<<";"<<endl;
    }

    agent.close();
}
//Criação da população inicial
void Search::criarPopulacaoInicial() {

    this->populacao= new Individuo *[this->conf->tamPop * 2];

    for(int i=0;i<this->conf->tamPop; i++){
        this->populacao[i] = new Individuo(true, this->conf, this->grammar);
    }
}
//Avalia a população
void Search::avaliarPopulacao(int initialIndex, int finalIndex, int generation) {

    this->conf->sementeGeracao=clock();
    gerarSolucoesIniciais();
    //Executando
    for(int i=initialIndex;i<finalIndex;i++){
        this->populacao[i]->nome= "gpAgent_g" + to_string(generation) + "_i"+ to_string(i);
        exportarIndividuo(i);

        cout<<this->populacao[i]->nome<<", ";
        clock_t time=clock();
        avaliarIndividuo(i);
        time=clock()-time;

        exportarResultadoIndividuo(i);
        cout <<"fitness: "+to_string(this->populacao[i]->fitness) +" -- Tempo: "+ to_string((double)time/CLOCKS_PER_SEC)+" s"<< endl;
    }


}
void Search::avaliarIndividuo(int index) {

    float fitness=0;
    float litSol;
    float *solucoes = new float[this->instancias.size()];
    double *tempos = new double[this->instancias.size()];

    //Linearizando e tookenizando o individuo
    string individuoLinear=this->populacao[index]->tree->str();
    vector<string> individuoTokenizado;
    tokenize(individuoLinear,individuoTokenizado," ");

    for(int i=0;i<this->instancias.size(); i++){

        //Executando
        Arena* arena=new Arena(&this->instancias.at(i));
        srand(this->conf->sementeGeracao);

        clock_t time= clock();
        arena->go(&individuoTokenizado);
        time=clock()-time;

        //Armazenando resultados
        solucoes[i]=arena->fo;
        litSol=this->instancias.at(i).litSol;
        tempos[i]=(double)time/CLOCKS_PER_SEC;

        //Atualizando fitness
        fitness+= (arena->fo - litSol) / litSol;

        delete arena;
    }
    fitness/=this->instancias.size();
    this->populacao[index]->fitness=fitness;
    this->populacao[index]->atualizarResultados(solucoes, tempos, this->instancias.size());

    delete [] solucoes;
    delete [] tempos;
}
bool Search::sortPopulationFitness(Individuo *a, Individuo *b) {
    return a->fitness < b->fitness;
}

void Search::tokenize(string str, vector<string> &token_v, string DELIMITER) {
    size_t start = str.find_first_not_of(DELIMITER), end=start;

    while (start != std::string::npos) {
        end = str.find(DELIMITER, start);
        token_v.push_back(str.substr(start, end-start));
        start = str.find_first_not_of(DELIMITER, end);
    }
}


