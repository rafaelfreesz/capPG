#include <iostream>
#include "Configuracoes.h"
#include "Gramatica.h"
#include "Search.h"

//Arrumada classe Instancia, preparar arena para interpretar comandos
using namespace std;

int main(int argc, char** argv) {
    int seed=clock();
    srand(seed);

    Configuracoes* conf = new Configuracoes(10, 1000, 100, 0.9, 0.1, 0.05, 2);

    Gramatica* gramatica=new Gramatica("grammar.dat", conf);

    Search* search = new Search(conf, gramatica, seed);

    search->inciar();

    delete conf;
}
