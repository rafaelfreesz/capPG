//
// Created by rafael on 21/02/2022.
//

#include "Grammar.h"
#define NONTERMINAL -1

//Constructor
Grammar::Grammar(string gram, Configures* conf) {
    this->conf=conf;
    makeTerminals();

    ifstream arq;
    string line;
    vector<string> tokens;

    arq.open(gram.c_str());

    int ntQty = 0;

    //Reads all productions and creates non-terminal Nodes, allocating all productions in tokens
    while(getline(arq, line)) {
        vector<string> rule; //Vetor de regras
        tokenize(line,rule, " ::= ");
        this->nonTerminals.push_back(rule.at(0));
        tokens.push_back(rule.at(1));

        //Create a rule node
        GrammarNode* n = new GrammarNode();
        n->type = -1;
        n->value = ntQty;
        n->mask = this->nonTerminals.at(ntQty);
        this->grammar.push_back(n);
        ntQty++;
    }

    //Transforming productions into Nodes
    for(int i = 0; i < tokens.size(); i++) {

        GrammarNode* n = grammar.at(i);

        //Non terminal i productions
        vector<string> productions;
        tokenize(tokens.at(i),productions, " | ");

        //Inserting one node for each production
        for(int j = 0; j < productions.size(); j++) {

            GrammarNode* nProd = new GrammarNode();
            n->productions.push_back(nProd);

            //Separating the elements of a single production into different strings
            vector<string> product;
            tokenize(productions.at(j),product, " ");

            //A child node for each production element
            for(int k = 0; k < product.size(); k++) {
                //Create the node of every product in a production
                auto t = getString(product.at(k));
                GrammarNode* nnn;

                if(get<0>(t) == NONTERMINAL) {
                    nnn = grammar.at(get<1>(t));
                } else {
                    nnn = new GrammarNode();
                    nnn->type = get<0>(t);
                    nnn->value = get<1>(t);
                    nnn->mask = get<2>(t);
                }
                nProd->productions.push_back(nnn);
            }
        }
    }

    arq.close();
    this->updateHigh();
}

//Destructor
Grammar::~Grammar() {

}

//Build Terminals
void Grammar::makeTerminals() {
    //0
    vector<string> constants;
    for(int i=0;i<=100;i++) {
        constants.push_back(to_string(0.05 * i));
    }
    this->terminals.push_back(constants);

    //2
    vector<string> binaryOperator;
    binaryOperator.push_back("+");
    binaryOperator.push_back("-");
    this->terminals.push_back(binaryOperator);
    //4
    vector<string> auxiliariesOperators;
    auxiliariesOperators.push_back("){");
    auxiliariesOperators.push_back("}");
    auxiliariesOperators.push_back(")");
    auxiliariesOperators.push_back(");");
    auxiliariesOperators.push_back(",");
    auxiliariesOperators.push_back(";");
    this->terminals.push_back(auxiliariesOperators);
    //5
    vector<string> capOperators;
    capOperators.push_back("swap(");
    capOperators.push_back("repart(");
    capOperators.push_back("shake(");
    capOperators.push_back("rec(");
    capOperators.push_back("RVND();");
    capOperators.push_back("bL1();");
    capOperators.push_back("bL2();");
    capOperators.push_back("bL3();");
    capOperators.push_back("bL4();");
    capOperators.push_back("blPc();");
    this->terminals.push_back(capOperators);

}

//Separate the string str by DELIMITER and insert into the &token_v vector
void Grammar::tokenize(string str, vector<string> &token_v, string DELIMITER) {
    size_t start = str.find_first_not_of(DELIMITER), end=start;

    while (start != std::string::npos) {
        end = str.find(DELIMITER, start);
        token_v.push_back(str.substr(start, end-start));
        start = str.find_first_not_of(DELIMITER, end);
    }
}

//Checks if an element is a non-terminal
bool Grammar::isNonTerminal(string s) {
    if(s.front() == '<' && s.back() == '>')
        return true;
    return false;
}

//Search for the index at which the referring non-terminal exists
double Grammar::getNonTerminal(string nt) {
    for(int i = 0; i < nonTerminals.size(); i++) {
        if(nt.compare(nonTerminals.at(i)) == 0)
            return i;
    }
    return -1;
}
//Returns the terminal coordinate, if any
tuple<double, double, string> Grammar::getTerminal(string t) {

    //Check if the terminal already exists
    for(int i=0;i<this->terminals.size(); i++){
        for(int j=0;j<this->terminals.at(i).size(); j++){
            if(this->terminals.at(i).at(j).compare(t) == 0){
                return make_tuple(i,j,t);
            }
        }
    }

    cout<<"error, "+ t<<endl;
    return make_tuple(-2,-2,"ERROR!");
}

//Transform a term into a tuple
tuple<double, double, string> Grammar::getString(string s) {
    tuple<double, double, string> t;
    if(isNonTerminal(s)) {
        double nt = getNonTerminal(s);
        if(nt != -1)
            return make_tuple(NONTERMINAL, nt, s);
    } else {
        t = getTerminal(s);
        if(get<0>(t) != -2)
            return t;
    }

    return make_tuple(-2.0, -2.0, "ERROR");
}
//Updating tree high
void Grammar::updateHigh() {
    high1(grammar.at(0));
    high2();

    for(GrammarNode* n : this->grammar) // sort the productions by minimum high it require
        stable_sort(n->productions.begin(), end(n->productions), sortNode);
}

//Calculates the minimum height of the grammar
void Grammar::high1(GrammarNode* n) {
    if(n->visited)
        return;

    n->visited = true;
    n->high = INFINT;

    if(n->type > NONTERMINAL)
        n->high = -1;

    for(GrammarNode* nn : n->productions) {

        int maxh = 0;
        for(GrammarNode* nnn : nn->productions) {
            high1(nnn);
            maxh = max(maxh, nnn->high);
        }
        nn->high = maxh;
        n->high = min(n->high, nn->high);
    }
    n->high++;
}

//Calculates the minimum height of the grammar
void Grammar::high2() {
    for(GrammarNode* n : this->grammar) {
        int min = INFINT;
        for (GrammarNode *nn: n->productions) {
            int max = 0;
            for (GrammarNode *nnn: nn->productions)
                if (nnn->high > max)
                    max = nnn->high;
            nn->high = max;

            if (min > max)
                min = max;
        }
    }

}

//Node sort criteria
bool Grammar::sortNode(GrammarNode* a, GrammarNode* b) {
    int aa = 0, bb = 0;

    for(GrammarNode* n : a->productions)
        if(aa < n->high)
            aa = n->high;

    for(GrammarNode* n : b->productions)
        if(bb < n->high)
            bb = n->high;

    return aa < bb;
}

//Derivation of node n
int Grammar::derivate(Node *n) {

    if(n->type==NONTERMINAL){
        n->height=0;

        //Looking for productions that do not reach the maximum depth of the tree
        int i=0;
        for(GrammarNode* prod : this->grammar.at(n->value)->productions){
            if(prod->high + n->deep + 1 <= this->conf->maxDeep){
                i++;
            }else{
                break;
            }
        }

        if(i==0){
            cout<<"Minimum tree height exception"<<endl;
            exit(43);
        }

        //Randomizing production among those available
        int prod=rand()%i;

        //Adding said production as a child of the current node
        for(GrammarNode* m : this->grammar.at(n->value)->productions.at(prod)->productions){
            Node* p = new Node(m->type, m->value, m->mask, n->deep + 1);

            p->index=n->sons.size();
            p->father=n;
            n->addSon(p);

            int h = derivate(p);

            if(h >= n->height){
                n->height=h;
            }
        }

    }
    return n->height+1;
}

//Build initial Population
int Grammar::buildInitialPopulation(Node *n, int deep) {
    if(n->type==NONTERMINAL) {
        n->height = 0;

        //Searching for productions of n that do not reach the maximum depth of the tree
        int i = 0;
        for (GrammarNode *prod: this->grammar.at(n->value)->productions) {
            if (prod->high + n->deep + 1 <= deep) {
                i++;
            } else {
                break;
            }
        }

        if (i == 0) {
            //Sort grammar nodes by height
            for (GrammarNode *n: this->grammar) {
                stable_sort(n->productions.begin(), end(n->productions), sortNode);
            }

            for (GrammarNode *prod: this->grammar.at(n->value)->productions) {
                if (prod->high + n->deep + 1 <= deep) {
                    i++;
                } else {
                    break;
                }
            }

        }

        if (i == 0) {
            cout<<"Minimum tree height exception"<<endl;
            exit(43);
        }

        //Randomizing production among those available
        int prod = rand() % i;

        //Adding referring production as child of current node
        for (GrammarNode *m: this->grammar.at(n->value)->productions.at(prod)->productions) {
            Node *p = new Node(m->type, m->value, m->mask, n->deep + 1);

            p->index = n->sons.size();
            p->father = n;
            n->addSon(p);

            int h = buildInitialPopulation(p, deep);

            if (h >= n->height) {
                n->height = h;
            }
        }
    }else{
        n->height=0;
    }
    return n->height+1;
}

//Print grammar
void Grammar::printGrammar() {

        for(GrammarNode* n : grammar) {
        cout << n->mask << "(" << n->high << ") " << " ::= ";

        for(GrammarNode* nn : n->productions) {
            for(GrammarNode* nnn : nn->productions) {
                cout << "(" << nnn->type << "," << nnn->value << "," << nnn->mask << "," << nnn->high << ") ";
            }
        }

        cout << endl;
    }

}

//Print terminals
void Grammar::printTerminals() {
    for(int i=0;i<this->terminals.size(); i++){
        for(int j=0;j<this->terminals.at(i).size(); j++){
            cout << this->terminals.at(i).at(j) << " ";
        }
        cout<<endl;
    }

}


