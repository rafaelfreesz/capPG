#include <iostream>
#include "Problema.h"
#include <vector>
#include "Entrada.h"
#include "Saida.h"
using namespace std;

int main(int argc, char** argv) {
    string instNome =string(argv[1]);

    Entrada *instancia = new Entrada("./Instancias/"+instNome);

    Saida* saida = new Saida("Resultados");

    int semente = stoi(argv[2]);
    srand(semente);

    Problema *problema = instancia->montarProblema();
    delete instancia;

    clock_t time = clock();
    problema->go();
    time = clock() - time;

    saida->imprimirResultado(problema, (double) time / CLOCKS_PER_SEC);

    delete problema;
    delete saida;
    return 0;
}
