//
// Created by rafael on 13/05/22.
//

#include "Sala.h"

Sala::Sala(){
    this->id=-1;
    this->comprimento=-1;
    this->alocada=false;
}
Sala::~Sala(){}

//---Getters&Setters---//
void Sala::setId(int id) {
    this->id=id;
}
void Sala::setComprimento(int comprimento) {
    this->comprimento=comprimento;
}
int Sala::getId() {
    return this->id;
}
int Sala::getComprimento() {

    return this->comprimento;
}
void Sala::alocar() {
    this->alocada=true;
}
void Sala::desalocar() {
    this->alocada=false;
}
bool Sala::estaAlocada() {
    return this->alocada;
}
