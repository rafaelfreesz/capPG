//
// Created by rafael on 21/02/2022.
//

#include "Parser.h"

Parser::Parser() {
    ifstream AGPAgentFile;
    AGPAgentFile.open("aGpAgent.parc");

    this->gpAgent << AGPAgentFile.rdbuf();

}
