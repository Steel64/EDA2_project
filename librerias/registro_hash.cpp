#include "registro_hash.h"

int TablaHash::hash(Reserva* reserva) {
    int valor = stoi(reserva->getIdReserva().substr(4));
    if (valor < 1) {
        throw invalid_argument("El ID debe ser positivo");
    }
    return (valor-1) % TAMANIO;
}

TablaHash::TablaHash() {
    for (int i = 0; i < TAMANIO; i++) {
        regHash[i] = nullptr;
    }
}

void TablaHash::insertar(Reserva* reserva) {
    int idx = hash(reserva);
    NodoHash* nuevo = new NodoHash();
    nuevo->reserva = reserva;
    nuevo->siguiente = regHash[idx];
    regHash[idx] = nuevo;
}

NodoHash* TablaHash::buscar(string idReserva) {
    for (int i = 0; i < TAMANIO; i++){
        NodoHash* actual = regHash[i];
        while (actual != nullptr) {
            if (actual->reserva->getIdReserva() == idReserva){
                return actual;
            }
            actual = actual->siguiente;
        }
    }
    return nullptr;
};

void TablaHash::imprimir() {
    for (int i = 0; i < TAMANIO; i++) {
        cout << "Indice " << i << ": ";
        NodoHash* actual = regHash[i];
        while (actual != nullptr) {
            cout << actual->reserva->getIdReserva() << " -> ";
            actual = actual->siguiente;
        }
        cout << "NULL" << endl;
    }
}