//
// Created by rafael on 22/02/2022.
//

#include "Search.h"
//Constructor
Search::Search(Configures *conf, int seed) {
    this->conf=conf;
    this->seed=seed;
    this->grammar=new Grammar("grammar.dat", conf);
    buildInstances();

}
//Destructor
Search::~Search() {
    delete this->conf;
}

//Build all instances and its attributes
void Search::buildInstances() {

    Instance* instance;
    ifstream fileStream;
    string line;

    //Importing instance name and literature solution
    line="./Instances/lit.sol";
    fileStream.open(line.c_str());
    while(getline(fileStream, line)){

        this->instances.emplace_back();
        instance=&this->instances.at(this->instances.size() - 1);


        vector<string> tokens;
        tokenize(line, tokens, ";");

        instance->name=tokens.at(0);
        instance->literatureCost=stof(tokens.at(1));

    }
    fileStream.close();

    //Importing instance data
    for(int i=0;i<this->instances.size(); i++){
        instance=&this->instances.at(i);
        line= "./Instances/" + instance->name;

        fileStream.open(line.c_str());

        getline(fileStream, line);
        instance->n= stoi(line);

        //Importing lengths
        instance->buildLengths();
        getline(fileStream, line);

        vector<string> lengthTokens;
        tokenize(line, lengthTokens, ",");

        for(int j=0; j < instance->n; j++){
            instance->lengths[j]= stoi(lengthTokens.at(j));
        }

        //Building demands
        instance->buildDemands();

        for(int j=0; j < instance->n; j++){
            getline(fileStream, line);
            vector<string> demTokens;
            tokenize(line, demTokens, ",");
            for(int k=0; k < instance->n; k++){
                instance->demands[j][k]=stoi(demTokens[k]);
            }
        }

        //checking consistency
        for(int i=0; i < instance->n; i++){
            for(int j=0; j < instance->n; j++){
                if(instance->demands[i][j] != instance->demands[j][i]){
                    cout<<"Demand synchronization error"<<endl;
                    exit(100);
                }
            }
        }

        fileStream.close();
    }
}

//Run individuals for each one of the 89 instances and make statistics
void Search::runAllInstances() {

    string sentence;
    ifstream individualFile;

    sentence="./PosTest/Best";
    individualFile.open(sentence.c_str());

    sentence ="./PosTest/Result";
    ofstream saida(sentence.c_str());

    sentence = "./PosTest/toLatex";
    ofstream toLatex(sentence.c_str());

    vector<string> latexBestStrs;
    vector<string> latexAvgStrs;
    for(int i=0;i<this->instances.size(); i++){
        latexBestStrs.push_back(this->instances.at(i).name);
        latexAvgStrs.push_back(this->instances.at(i).name);
    }

    //Generating seeds. One for each run
    vector<int> seed;
    sentence="seed: ";
    for(int i=0;i<30;i++){
        seed.push_back(rand());
        sentence+= to_string(seed.at(i)) + " ";
    }
    saida<<sentence<<endl;

    double* costs=new double [30];
    double* times=new double [30];
    double costAvg;
    double timeAvg;
    double bestCost;

    generateInitialSolutions();
    string outputStr;
    string instanceStr;
    string individualStr;
    //For the top 10 individuals of each instance
    while(getline(individualFile, instanceStr)){
        cout << "Generated individuals for instance " + instanceStr << endl;
        saida << "Individuals_Instance_" + instanceStr << endl;

        //For each individual
        for(int i=0;i<1;i++){ //TODO fix i limt
            getline(individualFile, individualStr);
            cout << "Individual " + individualStr << endl;
            saida << individualStr << endl;
            //Recovering Individual
            vector<string> tokenizedIndividual;
            sentence= "./PosTest/Populations/" + instanceStr + "/Population/" + individualStr;

            ifstream fileIndividuo;
            fileIndividuo.open(sentence.c_str());

            sentence="";
            string linearIndividual="";
            while(getline(fileIndividuo,sentence)){
                linearIndividual+=sentence;
            }

            tokenize(linearIndividual, tokenizedIndividual, " ");


            fileIndividuo.close();

            //for each instance
            for(int j=0;j<this->instances.size(); j++) {
                outputStr=this->instances.at(j).name;

                //Run 30 times
                costAvg=0;
                timeAvg=0;
                bestCost=0;
                for(int k=0;k<30;k++){
                    srand(seed.at(k));
                    Arena* arena=new Arena(&this->instances.at(j));


                    clock_t time=clock();
                    arena->go(&tokenizedIndividual);
                    time=clock()-time;

                    costs[k]=arena->cost;
                    times[k]= (double)time / CLOCKS_PER_SEC;
                    outputStr+= ";" + to_string(costs[k]) + ";" + to_string(times[k]);

                    delete arena;
                }
                for(int k=0;k<30;k++){
                    timeAvg+=times[k];
                    costAvg+=costs[k];
                    if(costs[k] < bestCost || bestCost == 0){
                        bestCost=costs[k];
                    }
                }
                timeAvg/=30.0;
                costAvg/=30.0;
                outputStr+= " bS: " + to_string(bestCost) + "; aS: " + to_string(costAvg) + "; aT: " + to_string(timeAvg);
                saida << outputStr << endl;

                latexBestStrs.at(j)+=" & "+ to_string(bestCost);
                latexAvgStrs.at(j)+=" & "+ to_string(costAvg);

                cout << this->instances.at(j).name + "- bS:" + to_string(bestCost) + "; aS:" << to_string(costAvg) << "; aT:" << to_string(timeAvg) << endl;

            }
        }

    }

    for(int i=0;i<this->instances.size(); i++){
        toLatex<<latexBestStrs.at(i);
    }
        toLatex<<endl<<endl;

    for(int i=0;i<this->instances.size(); i++){
        toLatex<<latexAvgStrs.at(i);
    }

    individualFile.close();
    delete[] costs;
    delete[] times;

}

//Running the genetic programming
void Search::evolve() {

    Output* generationsReport=new Output("Results");
    generationsReport->printLine("Run seed: " + to_string(this->seed));

    cout<<"-------Building Initial Population"<<endl;

    this->buildInitialPopulation();
    evaluatePopulation(0, this->conf->popSize, 0);
    stable_sort(this->population, this->population + conf->popSize, sortPopulationFitness);

    cout<<"--------Initial population built."<<endl;

    generationsReport->printResults(this->population, this->conf->popSize, 0, this->conf->seed);

    cout<<"Running generations:"<<endl;
    for(int it=1; it<this->conf->generations; it++){
        clock_t time=clock();
        this->conf->seed=clock();
        cout << "--------Generation " + to_string(it) + ", Seed: " + to_string(this->conf->seed) + " :" << endl;

        operate();

        evaluatePopulation(this->conf->popSize, this->conf->popSize * 2, it);

        stable_sort(this->population + this->conf->popSize, this->population + this-> conf->popSize * 2, sortPopulationFitness);


        replace();
        stable_sort(this->population, this->population + this->conf->popSize * 2, sortPopulationFitness);

        for(int i = this->conf->popSize; i < this->conf->popSize * 2; i++){
            delete this->population[i];
        }

        time=clock()-time;
        cout<<"--------Generation "+to_string(it)+" ends in "+ to_string((double)time/CLOCKS_PER_SEC)+" s."<<endl<<endl;
        generationsReport->printResults(this->population, this->conf->popSize, it, this->conf->seed);
    }
}

//Replacing old individuals with new evaluated individuals
void Search::replace() {
    for(int i = this->conf->popSize * this->conf->elitism, j = this->conf->popSize; i < this->conf->popSize; i++, j++) {
        swap(this->population[i], this->population[j]);
    }
}

//Crossover and mutation stage
void Search::operate() {
    int IndividualQty=this->conf->numParents;
    Individual ** selected;

    for(int i=this->conf->popSize; i < this->conf->popSize * 2; i+=IndividualQty){


        selected = new Individual *[IndividualQty];
        randParents(selected, i); //Selecionado pais

        crossover(selected);

        mutate(selected);

        //Clearing pointers
        for(int j = 0; j < IndividualQty; j++){
            selected[j] = nullptr;
        }
        delete selected;

    }


}
//Mutation operator
void Search::mutate(Individual **selected) {
    for(int i=0;i<conf->numParents; i++){
        if(rand()%101< this->conf->mutationRate * 100){
            auxMutate(selected[i]->tree);
        }
    }

}

//Mutation auxiliar operator
void Search::auxMutate(Tree *t) {
    Node* n = t->subTree();
    n->erase();
    this->grammar->derivate(n);
    t->update();
}
//Crossover operator
void Search::crossover(Individual **selected) {
    int individualQty=this->conf->numParents;

    for(int i=1; i < individualQty; i+=2){
        if(rand()%101< this->conf->crossoverRate * 100){
            treeCrossover(selected[i - 1]->tree, selected[i]->tree);

        }
    }

}

void Search::treeCrossover(Tree *a, Tree *b) {
    Node* n = a->subTree();
    Node* m = b->targetSubTree(n);

    if(n == NULL || m == NULL) {
        return;
    }
    swap(n->sons, m->sons);
    swap(n->father, m->father);

    a->update();
    b->update();
}

//Randomizing Parents
void Search::randParents(Individual **parents, int ind) {

    for(int i=0;i<this->conf->numParents; i+=2) {
        int x = rand() % this->conf->popSize;
        int y = rand() % this->conf->popSize;

        while (x == y) {
            y = rand() % this->conf->popSize;
        }

        parents[i]=this->population[x]->clone();
        parents[i + 1]=this->population[y]->clone();

        this->population[ind]=parents[i];
        this->population[ind + 1]=parents[i + 1];


    }

}

//Generating random individuals for generation 0
void Search::generateInitialSolutions() {
    for(int i=0;i<this->instances.size(); i++){
        this->instances[i].buildInitialSolution();
    }
}

//Print result to file
void Search::printIndividualResults(int index) {
    string line= "r_" + this->population[index]->name;
    ofstream agent("Population/Results/" + line);
    for(int i=0;i<this->instances.size(); i++){
        line= to_string(this->population[index]->costs[i]) + "; " + to_string(this->population[index]->times[i]);
        agent << line << endl;
    }
    agent.close();
}

//Print individual to file
void Search::printIndividual(int index) {
    string line=this->population[index]->name;
    ofstream agent("Population/" + line);

    line= this->population[index]->tree->str();
    vector<string> terminais;
    tokenize(line, terminais, ";");

    for(int i=0;i<terminais.size();i++){
        agent << terminais.at(i)<<";"<<endl;
    }

    agent.close();
}
//Building initial population
void Search::buildInitialPopulation() {

    this->population= new Individual *[this->conf->popSize * 2];

    for(int i=0;i<this->conf->popSize; i++){
        this->population[i] = new Individual(true, this->conf, this->grammar);
    }
}
//Evaluating population
void Search::evaluatePopulation(int initialIndex, int finalIndex, int generation) {

    this->conf->seed=clock();
    generateInitialSolutions();

    for(int i=initialIndex;i<finalIndex;i++){
        this->population[i]->name= "gpAgent_g" + to_string(generation) + "_i" + to_string(i);
        printIndividual(i);

        cout << this->population[i]->name << ", ";
        clock_t time=clock();
        evaluateIndividual(i);
        time=clock()-time;

        printIndividualResults(i);
        cout << "fitness: " + to_string(this->population[i]->fitness) + " -- Time: " + to_string((double)time / CLOCKS_PER_SEC) + " s" << endl;
    }


}

//Evaluate one individual
void Search::evaluateIndividual(int index) {

    float fitness=0;
    float literatureCost;
    float *costs = new float[this->instances.size()];
    double *times = new double[this->instances.size()];

    //Linearizing and tookenizing the individual
    string linearIndividual=this->population[index]->tree->str();
    vector<string> tokenizedIndividual;
    tokenize(linearIndividual, tokenizedIndividual, " ");

    for(int i=0;i<this->instances.size(); i++){

        //Running
        Arena* arena=new Arena(&this->instances.at(i));
        srand(this->conf->seed);

        clock_t time= clock();
        arena->go(&tokenizedIndividual);
        time=clock()-time;

        //Storing results
        costs[i]=arena->cost;
        literatureCost=this->instances.at(i).literatureCost;
        times[i]= (double)time / CLOCKS_PER_SEC;

        //Storing fitness
        fitness+= (arena->cost - literatureCost) / literatureCost;

        delete arena;
    }
    fitness/=this->instances.size();
    this->population[index]->fitness=fitness;
    this->population[index]->updateResults(costs, times, this->instances.size());

    delete [] costs;
    delete [] times;
}

//Individual sort criteria
bool Search::sortPopulationFitness(Individual *a, Individual *b) {
    return a->fitness < b->fitness;
}

//Tokenizing individuals
void Search::tokenize(string str, vector<string> &token_v, string DELIMITER) {
    size_t start = str.find_first_not_of(DELIMITER), end=start;

    while (start != std::string::npos) {
        end = str.find(DELIMITER, start);
        token_v.push_back(str.substr(start, end-start));
        start = str.find_first_not_of(DELIMITER, end);
    }
}


