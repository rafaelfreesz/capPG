//
// Created by rafael on 21/02/2022.
//

#include "Gramatica.h"
#define NONTERMINAL -1
Gramatica::Gramatica(string gram, Configuracoes* conf) {
    this->conf=conf;
    makeTerminals();

    ifstream arq;
    string line;
    vector<string> tokens;

    arq.open(gram.c_str());

    int ntQty = 0;

    //Lê todas as produções já criando os Nodes de não terminais, alocando todas as produções em tokens
    while(getline(arq, line)) {
        vector<string> rule; //Vetor de regras
        tokenize(line,rule, " ::= ");
        this->naoTerminais.push_back(rule.at(0));
        tokens.push_back(rule.at(1));

        //create a rule node
        Node* n = new Node();
        n->type = -1;
        n->value = ntQty;
        n->mask = this->naoTerminais.at(ntQty);
        this->gramatica.push_back(n);
        ntQty++;
    }

    //Transformando as produções em Nodes
    for(int i = 0; i < tokens.size(); i++) {

        Node* n = gramatica.at(i);

        //Produções do não terminal i
        vector<string> production;
        tokenize(tokens.at(i),production, " | ");

        //Inserindo em n->productions um nó pra cada produção
        for(int j = 0; j < production.size(); j++) {

            Node* nn = new Node();
            n->productions.push_back(nn);

            //Separando os elementos de uma unica produção em strings diferentes
            vector<string> product;
            tokenize(production.at(j),product, " ");

            //um nó filho para cada elemento da produção
            for(int k = 0; k < product.size(); k++) {
                //Create the node of every product in a production
                auto t = getString(product.at(k));
                Node* nnn;

                //Caso seja nao teminal
                if(get<0>(t) == NONTERMINAL) {
                    nnn = gramatica.at(get<1>(t));
                } else {
                    nnn = new Node();
                    nnn->type = get<0>(t);
                    nnn->value = get<1>(t);
                    nnn->mask = get<2>(t);
                }
                nn->productions.push_back(nnn);
            }
        }
    }

    arq.close();
    this->updateHigh();
}

Gramatica::~Gramatica() {

}

void Gramatica::makeTerminals() {
    //0
    vector<string> constantes;
    for(int i=0;i<=100;i++) {
        constantes.push_back(to_string(0.05 * i));
    }
    this->terminais.push_back(constantes);

    //2
    vector<string> operadoresAritmeticosBinarios;
    operadoresAritmeticosBinarios.push_back("+");
    operadoresAritmeticosBinarios.push_back("-");
    this->terminais.push_back(operadoresAritmeticosBinarios);
    //4
    vector<string> operadoresAuxiliares;
    operadoresAuxiliares.push_back("){");
    operadoresAuxiliares.push_back("}");
    operadoresAuxiliares.push_back(")");
    operadoresAuxiliares.push_back(");");
    operadoresAuxiliares.push_back(",");
    operadoresAuxiliares.push_back(";");
    this->terminais.push_back(operadoresAuxiliares);
    //5
    vector<string> operadoresCap;
    operadoresCap.push_back("swap(");
    operadoresCap.push_back("repart(");
    operadoresCap.push_back("shake(");
    operadoresCap.push_back("rec(");
    operadoresCap.push_back("RVND();");
    operadoresCap.push_back("bL1();");
    operadoresCap.push_back("bL2();");
    operadoresCap.push_back("bL3();");
    operadoresCap.push_back("bL4();");
    operadoresCap.push_back("blPc();");
    this->terminais.push_back(operadoresCap);

}

//Separa a string str por DELIMITER e insere no vector &token_v
void Gramatica::tokenize(string str, vector<string> &token_v, string DELIMITER) {
    size_t start = str.find_first_not_of(DELIMITER), end=start;

    while (start != std::string::npos) {
        end = str.find(DELIMITER, start);
        token_v.push_back(str.substr(start, end-start));
        start = str.find_first_not_of(DELIMITER, end);
    }
}

//Verifica se um elemento é um não terminal
bool Gramatica::isNonTerminal(string s) {
    if(s.front() == '<' && s.back() == '>')
        return true;
    return false;
}

//Busca o indice no qual o nao terminal referente existe
double Gramatica::getNonTerminal(string nt) {
    for(int i = 0; i < naoTerminais.size(); i++) {
        if(nt.compare(naoTerminais.at(i)) == 0)
            return i;
    }
    return -1;
}
//Retorna a coordenada do terminal, caso exista
tuple<double, double, string> Gramatica::getTerminal(string t) {

    //Verifica se o terminal ja existe
    for(int i=0;i<this->terminais.size(); i++){
        for(int j=0;j<this->terminais.at(i).size(); j++){
            if(this->terminais.at(i).at(j).compare(t) == 0){
                return make_tuple(i,j,t);
            }
        }
    }

    cout<<"error, "+ t<<endl;
    return make_tuple(-2,-2,"ERRO!");
}
//Transforma um termo em uma tupla
tuple<double, double, string> Gramatica::getString(string s) {
    tuple<double, double, string> t; //<terminal?,ja existe?,mask>
    if(isNonTerminal(s)) {
        double nt = getNonTerminal(s);
        if(nt != -1)
            return make_tuple(NONTERMINAL, nt, s);
    } else {
        t = getTerminal(s);
        if(get<0>(t) != -2)
            return t;
    }

    return make_tuple(-2.0, -2.0, "ERRO");
}

void Gramatica::updateHigh() {
    high1(gramatica.at(0));
    high2();

    for(Node* n : this->gramatica) // sort the productions by minimum high it require
        stable_sort(n->productions.begin(), end(n->productions), sortNode);
}

//Calcula a altura minima da gramática
void Gramatica::high1(Node* n) {
    if(n->visited)
        return;

    n->visited = true;
    n->high = INFINT;

    if(n->type > NONTERMINAL)
        n->high = -1;

    for(Node* nn : n->productions) {

        int maxh = 0;
        for(Node* nnn : nn->productions) {
            high1(nnn);
            maxh = max(maxh, nnn->high);
        }
        nn->high = maxh;
        n->high = min(n->high, nn->high);
    }
    n->high++;
}
void Gramatica::high2() {
    for(Node* n : this->gramatica) {
        int min = INFINT;
        for (Node *nn: n->productions) {
            int max = 0;
            for (Node *nnn: nn->productions)
                if (nnn->high > max)
                    max = nnn->high;
            nn->high = max;

            if (min > max)
                min = max;
        }
    }

}
bool Gramatica::sortNode(Node* a, Node* b) {
    int aa = 0, bb = 0;

    for(Node* n : a->productions)
        if(aa < n->high)
            aa = n->high;

    for(Node* n : b->productions)
        if(bb < n->high)
            bb = n->high;

    return aa < bb;
}

//Derivação do nó n
int Gramatica::derivate(No *n) {

    if(n->type==NONTERMINAL){
        n->height=0;

        //Buscando produções que nao atinjam a produndidade maxima da arvore
        int i=0;
        for(Node* prod : this->gramatica.at(n->value)->productions){
            if(prod->high + n->deep + 1 <= this->conf->profMaxima){
                i++;
            }else{
                break;
            }
        }

        if(i==0){
            cout<<"Altura minima de arvore exceprion"<<endl;
            exit(43);
        }

        //Randomizando produção dentre as disponiveis;
        int prod=rand()%i;

        //Adicionando produção referente como filha do no atual
        for(Node* m : this->gramatica.at(n->value)->productions.at(prod)->productions){
            No* p = new No(m->type,m->value,m->mask,n->deep+1);

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
//Criando a população inicial
int Gramatica::criarPopulacaoInicial(No *n, int deep) {
    if(n->type==NONTERMINAL) {
        n->height = 0;

        //Buscando produções de n que nao atinjam a produndidade maxima da arvore
        int i = 0;
        for (Node *prod: this->gramatica.at(n->value)->productions) {
            if (prod->high + n->deep + 1 <= deep) {
                i++;
            } else {
                break;
            }
        }

        if (i == 0) {
            //Ordena os nos da gramática por altura minima
            for (Node *n: this->gramatica) {
                stable_sort(n->productions.begin(), end(n->productions), sortNode);
            }

            for (Node *prod: this->gramatica.at(n->value)->productions) {
                if (prod->high + n->deep + 1 <= deep) {
                    i++;
                } else {
                    break;
                }
            }

        }

        if (i == 0) {
            cout << "Altura minima de arvore exceprion" << endl;
            exit(43);
        }

        //Randomizando produção dentre as disponiveis;
        int prod = rand() % i;

        //Adicionando produção referente como filha do no atual
        for (Node *m: this->gramatica.at(n->value)->productions.at(prod)->productions) {
            No *p = new No(m->type, m->value, m->mask, n->deep + 1);

            p->index = n->sons.size();
            p->father = n;
            n->addSon(p);

            int h = criarPopulacaoInicial(p, deep);

            if (h >= n->height) {
                n->height = h;
            }
        }
    }else{
        n->height=0;
    }
    return n->height+1;
}

//-----------Test Functions-----------------//
void Gramatica::printGrammar() {

        for(Node* n : gramatica) {
        cout << n->mask << "(" << n->high << ") " << " ::= ";
//        cout << n->mask << " ::= ";

        for(Node* nn : n->productions) {
            for(Node* nnn : nn->productions) {
                cout << "(" << nnn->type << "," << nnn->value << "," << nnn->mask << "," << nnn->high << ") ";
//                cout << nnn->mask << " ";
            }
//            cout << "| ";
        }

        cout << endl;
    }

}

void Gramatica::printProductions(Node* nonTerminalNode) {
    for(int i=0;i<nonTerminalNode->productions.size();i++){

    }

}

void Gramatica::printTerminals() {
    for(int i=0;i<this->terminais.size(); i++){
        for(int j=0;j<this->terminais.at(i).size(); j++){
            cout << this->terminais.at(i).at(j) << " ";
        }
        cout<<endl;
    }

}


