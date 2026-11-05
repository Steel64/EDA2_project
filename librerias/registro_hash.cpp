#include "registro_hash.h"

int TablaHash::hash(int id) {
    if (id < 1) {
        throw invalid_argument("El ID debe ser positivo");
    }
    return (id-1) % TAMANIO;
}

TablaHash::TablaHash() {
    for (int i = 0; i < TAMANIO; i++) {
        tabla[i] = nullptr;
    }
}

void TablaHash::insertar(int id, Reserva * reserva) {
    int idx = hash(id);
    Nodo* nuevo = new Nodo();
    nuevo->id = id;
    nuevo->reserva = reserva;
    nuevo->siguiente = tabla[idx];
    tabla[idx] = nuevo;
}

Nodo* TablaHash::buscar(string idReserva) {
    int id = stoi(idReserva.substr(4));
    int valor = hash(id);
    Nodo* actual = tabla[valor];
    while (actual != nullptr) {
        if (actual->id == id){
            return actual;
        }
        actual = actual->siguiente;
    }
    return nullptr;
};

void TablaHash::imprimir() {
    for (int i = 0; i < TAMANIO; i++) {
        cout << "Indice " << i << ": ";
        Nodo* actual = tabla[i];
        while (actual != nullptr) {
            cout << actual->reserva->getIdReserva() << " -> ";
            actual = actual->siguiente;
        }
        cout << "NULL" << endl;
    }
}