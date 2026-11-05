#include <iostream>
#include <string>
#include "../base_datos/reservas.h"
using namespace std;

// usar para reservas_pequeñas
const int TAMANIO = 10;
// usar para reservas_grandes
//const int TAMANIO = 2000;

struct Nodo {
    int id;
    Reserva* reserva;
    Nodo* siguiente;
};

class TablaHash {
private:
    Nodo* tabla[TAMANIO];

    int hash(int id);

public:
    TablaHash();

    void insertar(int id, Reserva * reserva);

    Nodo* buscar(string idReserva);

    void imprimir();
};