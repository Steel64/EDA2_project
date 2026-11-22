#pragma once
#include <iostream>
#include <string>
#include "../base_datos/reservas.h"
using namespace std;


// Tabla dimensionada para 10,000 reservas (factor de carga cercano a 1).
const int TAMANIO = 10000;

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
