#include <iostream>
#include "Configures.h"
#include "Search.h"

using namespace std;

int main(int argc, char** argv) {

    int seed=clock();
    srand(seed);

    Configures* conf = new Configures(10, 100, 100, 0.9, 0.1, 0.05, 2);

    Search* search = new Search(conf, seed);

    search->inciar();
    search->iniciarPosTeste();
    delete conf;
}


