//
// Created by rafael on 13/05/22.
//

#ifndef ARENAPG_SALA_H
#define ARENAPG_SALA_H

class Sala {
public:
    Sala();
    ~Sala();

    void setId(int id);
    void setComprimento(int comprimento);

    int getId();
    int getComprimento();

    void alocar();
    void desalocar();
    bool estaAlocada();
private:
    int id;
    int comprimento;
    bool alocada;

};


#endif //ARENAPG_SALA_H
