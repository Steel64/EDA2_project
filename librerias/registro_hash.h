#pragma once
#include <iostream>
#include <string>
#include "../base_datos/reservas.h"
using namespace std;

// usar para csv de reservas_pequeñas
//const int TAMANIO = 10;
// usar para csv de reservas_masivo
const int TAMANIO = 2000;

struct NodoHash {
    Reserva* reserva;
    NodoHash* siguiente;
};

class TablaHash {
private:
    NodoHash* regHash[TAMANIO];
    int total = 0;

    int hash(const string& id);

public:
    TablaHash();
    ~TablaHash();
    bool eliminar(const string& id);

    bool insertar(Reserva* reserva);
    int cantidad() const;
    double factorCarga() const;
    int cadenaMasLarga() const;

    NodoHash* buscar(string idReserva);

};
