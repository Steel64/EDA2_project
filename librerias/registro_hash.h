#include <iostream>
#include <string>
#include "../base_datos/reservas.h"
using namespace std;

// usar para reservas_pequeñas
const int TAMANIO = 10;
// usar para reservas_grandes
//const int TAMANIO = 2000;

struct NodoHash {
    Reserva* reserva;
    NodoHash* siguiente;
};

class TablaHash {
private:
    NodoHash* regHash[TAMANIO];

    int hash(Reserva* reserva);

public:
    TablaHash();

    void insertar(Reserva* reserva);

    NodoHash* buscar(string idReserva);

    void imprimir();
};