//
// Created by rafael on 22/02/2022.
//

#include "Search.h"

Search::Search(Configures *conf, int seed) {
    this->conf=conf;
    this->seed=seed;
    this->grammar=new Grammar("grammar.dat", conf);
    montarInstancias();

}

Search::~Search() {
    delete this->conf;
}

void Search::montarInstancias() {

    Instance* instancia;
    ifstream fileStream;
    string linha;

    //Importando o name da instancia e a solução da literatura
    linha="./Instancias/lit.sol";
    fileStream.open(linha.c_str());
    while(getline(fileStream, linha)){

        this->instancias.emplace_back();
        instancia=&this->instancias.at(this->instancias.size()-1);


        vector<string> tokens;
        tokenize(linha, tokens, ";");

        instancia->name=tokens.at(0);
        instancia->literatureCost=stof(tokens.at(1));

    }
    fileStream.close();

    //Importando os dados da instancia
    for(int i=0;i<this->instancias.size();i++){
        instancia=&this->instancias.at(i);
        linha="./Instancias/"+instancia->name;

        fileStream.open(linha.c_str());

        getline(fileStream,linha);
        instancia->n= stoi(linha);

        //Importando lengths
        instancia->buildLengths();
        getline(fileStream,linha);

        vector<string> comprTokens;
        tokenize(linha, comprTokens, ",");

        for(int j=0;j<instancia->n;j++){
            instancia->lengths[j]= stoi(comprTokens.at(j));
        }

        //Importando demands
        instancia->buildDemands();

        for(int j=0;j<instancia->n;j++){
            getline(fileStream,linha);
            vector<string> demTokens;
            tokenize(linha, demTokens, ",");
            for(int k=0;k<instancia->n;k++){
                instancia->demands[j][k]=stoi(demTokens[k]);
            }
        }

        //verificando consistência
        for(int i=0;i<instancia->n;i++){
            for(int j=0;j<instancia->n;j++){
                if(instancia->demands[i][j] != instancia->demands[j][i]){
                    cout<<"Erro de sincronia das demands"<<endl;
                    exit(100);
                }
            }
        }

        fileStream.close();
    }
}


void Search::iniciarPosTeste() {

    string sentence;
    ifstream fileIndividuos;

    sentence="./PosTest/Melhores";
    fileIndividuos.open(sentence.c_str());

    sentence ="./PosTest/Resultado";
    ofstream saida(sentence.c_str());

    sentence = "./PosTest/toLatex";
    ofstream toLatex(sentence.c_str());

    vector<string> latexBestStrs;
    vector<string> latexAvgStrs;
    for(int i=0;i<this->instancias.size();i++){
        latexBestStrs.push_back(this->instancias.at(i).name);
        latexAvgStrs.push_back(this->instancias.at(i).name);
    }

    //Gerando sementes. Uma pra cada execução
    vector<int> sementes;
    sentence="sementes: ";
    for(int i=0;i<30;i++){
        sementes.push_back(rand());
        sentence+= to_string(sementes.at(i))+" ";
    }
    saida<<sentence<<endl;

    double* solucao=new double [30];
    double* tempo=new double [30];
    double mediaSolucao;
    double mediaTempo;
    double melhorSolucao;

    gerarSolucoesIniciais();
    string strSaida;
    string strInstancia;
    string strIndividuo;
    //Pros 10 melhores individuos de cada instancia
    while(getline(fileIndividuos,strInstancia)){
        cout<<"Individuos gerados para a instancia "+strInstancia<<endl;
        saida<<"Individuos_Instancia_"+strInstancia<<endl;

        //Pra cada individuo
        for(int i=0;i<1;i++){
            getline(fileIndividuos, strIndividuo);
            cout<<"Individuo "+strIndividuo<<endl;
            saida<<strIndividuo<<endl;
            //Recuperando Individuo
            vector<string> individuoTokenizado;
            sentence="./PosTest/Populacoes/"+ strInstancia + "/Population/" + strIndividuo;

            ifstream fileIndividuo;
            fileIndividuo.open(sentence.c_str());

            sentence="";
            string individuoLinear="";
            while(getline(fileIndividuo,sentence)){
                individuoLinear+=sentence;
            }

            tokenize(individuoLinear,individuoTokenizado," ");


            fileIndividuo.close();

            //pra cada instancia
            for(int j=0;j<this->instancias.size();j++) {
                strSaida=this->instancias.at(j).name;
                //Executar 30 vezes
                mediaSolucao=0;
                mediaTempo=0;
                melhorSolucao=0;
                for(int k=0;k<30;k++){
                    srand(sementes.at(k));
                    Arena* arena=new Arena(&this->instancias.at(j));


                    clock_t time=clock();
                    arena->go(&individuoTokenizado);
                    time=clock()-time;

                    solucao[k]=arena->fo;
                    tempo[k]=(double)time/CLOCKS_PER_SEC;
                    strSaida+= ";"+to_string(solucao[k])+";"+ to_string(tempo[k]);

                    delete arena;
                }
                for(int k=0;k<30;k++){
                    mediaTempo+=tempo[k];
                    mediaSolucao+=solucao[k];
                    if(solucao[k]<melhorSolucao || melhorSolucao==0){
                        melhorSolucao=solucao[k];
                    }
                }
                mediaTempo/=30.0;
                mediaSolucao/=30.0;
                strSaida+= " bS: "+to_string(melhorSolucao)+"; aS: "+ to_string(mediaSolucao)+"; aT: "+ to_string(mediaTempo);
                saida<<strSaida<<endl;

                latexBestStrs.at(j)+=" & "+ to_string(melhorSolucao);
                latexAvgStrs.at(j)+=" & "+ to_string(mediaSolucao);

                cout << this->instancias.at(j).name + "- bS:" + to_string(melhorSolucao) + "; aS:" << to_string(mediaSolucao) << "; aT:" << to_string(mediaTempo) << endl;

            }
        }

    }

    for(int i=0;i<this->instancias.size();i++){
        toLatex<<latexBestStrs.at(i);
    }
        toLatex<<endl<<endl;

    for(int i=0;i<this->instancias.size();i++){
        toLatex<<latexAvgStrs.at(i);
    }

    fileIndividuos.close();
    delete[] solucao;
    delete[] tempo;

}
void Search::inciar() {

    Saida* relatorioGeracoes=new Saida("Resultados");
    relatorioGeracoes->escreverLinha("Semente de execução: "+ to_string(this->seed));

    cout<<"-------Criando população inicial"<<endl;

    this->criarPopulacaoInicial();
    avaliarPopulacao(0, this->conf->popSize, 0);
    stable_sort(this->populacao, this->populacao + conf->popSize, sortPopulationFitness);

    cout<<"--------População Inicial Criada."<<endl;

    relatorioGeracoes->imprimirResultado(this->populacao, this->conf->popSize, 0, this->conf->seed);

    double tempoGeracao;
    cout<<"Iniciando gerações:"<<endl;
    for(int it=1; it<this->conf->generations; it++){
        clock_t time=clock();
        this->conf->seed=clock();
        cout << "--------Geração " + to_string(it) + ", Semente: " + to_string(this->conf->seed) + " :" << endl;

        operate();

        avaliarPopulacao(this->conf->popSize, this->conf->popSize * 2, it);

        stable_sort(this->populacao + this->conf->popSize, this->populacao + this-> conf->popSize * 2, sortPopulationFitness);


        replace();
        stable_sort(this->populacao, this->populacao + this->conf->popSize * 2, sortPopulationFitness);

        for(int i = this->conf->popSize; i < this->conf->popSize * 2; i++){
            delete this->populacao[i];
        }

        time=clock()-time;
        cout<<"--------Fim da Geração "+to_string(it)+" em "+ to_string((double)time/CLOCKS_PER_SEC)+" s."<<endl<<endl;
        relatorioGeracoes->imprimirResultado(this->populacao, this->conf->popSize, it, this->conf->seed);
    }
}

void Search::replace() {
    for(int i = this->conf->popSize * this->conf->elitism, j = this->conf->popSize; i < this->conf->popSize; i++, j++) {
        swap(this->populacao[i], this->populacao[j]);
    }
}
//Realização de cruamemento e mutação
void Search::operate() {
    int numIndividuos=this->conf->numParents;
    Individuo ** selecionados;

    for(int i=this->conf->popSize; i < this->conf->popSize * 2; i+=numIndividuos){


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
    for(int i=0;i<conf->numParents; i++){
        if(rand()%101< this->conf->mutationRate * 100){
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
void Search::crossover(Individuo **selecionados) {
    int numIndividuos=this->conf->numParents;

    for(int i=1;i<numIndividuos;i+=2){
        if(rand()%101< this->conf->crossoverRate * 100){
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
void Search::randPais(Individuo **pais, int ind) {

    for(int i=0;i<this->conf->numParents; i+=2) {
        int x = rand() % this->conf->popSize;
        int y = rand() % this->conf->popSize;

        while (x == y) {
            y = rand() % this->conf->popSize;
        }

        pais[i]=this->populacao[x]->clone();
        pais[i+1]=this->populacao[y]->clone();

        this->populacao[ind]=pais[i];
        this->populacao[ind + 1]=pais[i + 1];


    }

}
void Search::gerarSolucoesIniciais() {
    for(int i=0;i<this->instancias.size();i++){
        this->instancias[i].buildInitialSolution();
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

    this->populacao= new Individuo *[this->conf->popSize * 2];

    for(int i=0;i<this->conf->popSize; i++){
        this->populacao[i] = new Individuo(true, this->conf, this->grammar);
    }
}
//Avalia a população
void Search::avaliarPopulacao(int initialIndex, int finalIndex, int generation) {

    this->conf->seed=clock();
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
        srand(this->conf->seed);

        clock_t time= clock();
        arena->go(&individuoTokenizado);
        time=clock()-time;

        //Armazenando resultados
        solucoes[i]=arena->fo;
        litSol=this->instancias.at(i).literatureCost;
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


