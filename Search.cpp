//
// Created by rafael on 22/02/2022.
//

#include "Search.h"

Search::Search(Parser *parser, int *populationReplacement, Configures *conf, Grammar* grammar) {
    this->parser=parser;
    this->populationReplacement=populationReplacement;
    this->conf=conf;
    this->grammar=grammar;
    this->instances=new Instances();
    this->logFile=new Saida("logFile");

}

Search::~Search() {
    delete this->logFile;
}
void Search::exportGeneration(int gen) {
    string command="cp -r ./Population ./Generations/"+to_string(gen);
    system(command.c_str());
}
void Search::evolve() {

    ifstream File;
    File.open("AgpAgent.parc");
    this->saGpAgent << File.rdbuf();
    File.close();

    File.open("BgpAgent.parc");
    this->sbGpAgent << File.rdbuf();
    File.close();

    this->aGpAgent=this->saGpAgent.str();
    this->aGpAgent.pop_back();

    this->bGpAgent=this->sbGpAgent.str();
    this->bGpAgent.pop_back();

    Saida* relEnd=new Saida("Resultados");

    cout<<"-------Criando população inicial"<<endl;
    this->createInicialPopulation();
    evaluatePopulation(0,this->conf->popSize);

    stable_sort(this->pop,this->pop+conf->popSize,sortPopulationFitness);

    cout<<"--------População Inicial Criada."<<endl;
    exportGeneration(0);
    relEnd->imprimirResultado(this->pop, this->conf->popSize, 0, this->conf->genSeed);

    conf->optimizationEvaluations=100;

    cout<<"Iniciando gerações:"<<endl;
    for(int it=1; it<this->conf->evaluations;it++){
        this->conf->genSeed=clock();
        cout<<"--------Geração "+to_string(it)+", Semente: "+to_string(this->conf->genSeed)+" :"<<endl;

        operate();

        evaluatePopulation(this->conf->popSize,this->conf->popSize*2);

        stable_sort(this->pop + this->conf->popSize, this->pop +this-> conf->popSize * 2, sortPopulationFitness);


        replace();
        stable_sort(this->pop, this->pop + this->conf->popSize * 2, sortPopulationFitness);

        for(int i = this->conf->popSize; i < this->conf->popSize * 2; i++){
            delete this->pop[i];
        }

        cout<<"--------Fim da Geração "+to_string(it)+"."<<endl<<endl;
        exportGeneration(it);
        relEnd->imprimirResultado(this->pop, this->conf->popSize, it, this->conf->genSeed);
    }

}

void Search::replace() {
    for(int i = this->conf->popSize * this->conf->elitism, j = this->conf->popSize; i < this->conf->popSize; i++, j++) {
        swap(this->pop[i], this->pop[j]);
    }
}
//Realização de cruamemento e mutação
void Search::operate() {
    int numIndividuos=this->conf->numIndividuosSelection;
    Subject ** selecionados;

    for(int i=this->conf->popSize; i<this->conf->popSize*2;i+=numIndividuos){


        selecionados = new Subject*[numIndividuos];
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
void Search::mutate(Subject **selecionados) {
    for(int i=0;i<conf->numIndividuosSelection;i++){
        if(rand()%101<this->conf->mutationRate*100){
            auxMutate(selecionados[i]->tree);
        }
    }

}
void Search::auxMutate(Tree *t) {
    No* n = t->subTree();
    n->erase();
    this->grammar->derivate(n);
    t->update();
}
//Operação de crossover
void Search::crossover(Subject **selecionados) {
    int numIndividuos=this->conf->numIndividuosSelection;

    for(int i=1;i<numIndividuos;i+=2){
        if(rand()%101<this->conf->crossoverRate*100){
            treeCrossover(selecionados[i-1]->tree,selecionados[i]->tree);

        }
    }

}

void Search::treeCrossover(Tree *a, Tree *b) {
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
void Search::randPais(Subject **pais, int ind) {

    for(int i=0;i<this->conf->numIndividuosSelection;i+=2) {
        int x = rand() % this->conf->popSize;
        int y = rand() % this->conf->popSize;

        while (x == y) {
            y = rand() % this->conf->popSize;
        }

        pais[i]=this->pop[x]->clone();
        pais[i+1]=this->pop[y]->clone();

        this->pop[ind]=pais[i];
        this->pop[ind+1]=pais[i+1];


    }

}

//Criação da população inicial
void Search::createInicialPopulation() {

    this->pop=new Subject*[this->conf->popSize*2];

    for(int i=0;i<this->conf->popSize;i++){
        this->pop[i] = new Subject(true, this->conf,this->grammar);
    }
}
//Avalia a população
void Search::evaluatePopulation(int initialIndex, int finalIndex) {

    //Gerando os arquivos dos individuos, para que possam ser compilados
    for(int i=initialIndex;i<finalIndex;i++){
        this->pop[i]->subjectName="gpAgent"+ to_string(i);
        ofstream agent("Population/"+this->pop[i]->subjectName+".cpp");
        agent << this->aGpAgent + this->pop[i]->tree->str() + this->bGpAgent;
        agent.close();
    }

    this->conf->genSeed=clock();
    //Executando
    string execStr;
    for(int i=initialIndex;i<finalIndex;i++){
        cout<<"gpAgent"+to_string(i)<<", ";
        //Levando o individuo para arena com nome Problema.cpp
        this->conf->evaluations++;
        execStr="cp ./Population/gpAgent" + to_string(i) +".cpp ./arena/Problema.cpp"; //Copiando pra Arena
        this->logFile->escreverLinha(execStr);
        system(execStr.c_str());

        //Removendo o arquivo Resultados
        execStr="rm arena/Saidas/Resultados";
        this->logFile->escreverLinha(execStr);
        system(execStr.c_str());

        execute();
        //Lendo o score
        evaluateIndividual(i);
        cout<<"fitness: "+to_string(this->pop[i]->fitness)<<endl;
    }


}
void Search::execute() const {
    string execStr;

    for(int i=0;i<this->instances->n;i++){
        execStr="cd ./arena; g++ *.cpp -O3; ./a.out "+this->instances->names.at(i) +" "+to_string(this->conf->genSeed);
        this->logFile->escreverLinha(execStr);
        system(execStr.c_str());
    }
}
void Search::evaluateIndividual(int index) {

    ifstream File;
    File.open("./arena/Saidas/Resultados");
    string line;
    float fitness=0;
    float score;
    float litScore;
    for(int i=0;i<this->instances->n;i++){
        getline(File,line);
        vector<string> tokens;
        tokenize(line,tokens," ");
        litScore=this->instances->litSol.at(i);
        score=stof(tokens.at(0));
        fitness+= (score-litScore)/litScore;
    }
    fitness/=this->instances->n;
    this->pop[index]->fitness=fitness;
    File.close();

}
bool Search::sortPopulationFitness(Subject *a, Subject *b) {
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

