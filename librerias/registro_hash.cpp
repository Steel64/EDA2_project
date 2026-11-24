#include "registro_hash.h"

int TablaHash::hash(const string& id) {
    if (id.size() <= 4 || id.substr(0, 4) != "RES-") return -1;
    int valor = 0;
    bool positivo = false;
    for (size_t i = 4; i < id.size(); ++i) {
        if (id[i] < '0' || id[i] > '9') return -1;
        if (id[i] != '0') positivo = true;
        valor = (valor * 10 + id[i] - '0') % TAMANIO;
    }
    return positivo ? (valor + TAMANIO - 1) % TAMANIO : -1;
}

TablaHash::TablaHash() {
    for (int i = 0; i < TAMANIO; i++) {
        regHash[i] = nullptr;
    }
}

bool TablaHash::insertar(Reserva* reserva) {
    if (!reserva) return false;
    int idx = hash(reserva->getIdReserva());
    if (idx < 0 || buscar(reserva->getIdReserva())) return false;
    NodoHash* nuevo = new NodoHash();
    nuevo->reserva = reserva;
    nuevo->siguiente = regHash[idx];
    regHash[idx] = nuevo;
    ++total;
    return true;
}

int TablaHash::cantidad() const { return total; }
double TablaHash::factorCarga() const { return static_cast<double>(total) / TAMANIO; }
int TablaHash::cadenaMasLarga() const {
    int mayor = 0;
    for (int i=0; i<TAMANIO; ++i) {
        int largo = 0;
        for (NodoHash* n=regHash[i]; n; n=n->siguiente) ++largo;
        if (largo > mayor) mayor = largo;
    }
    return mayor;
}

NodoHash* TablaHash::buscar(string idReserva) {
    int idx = hash(idReserva);
    if (idx < 0) return nullptr;
    for (NodoHash* n = regHash[idx]; n; n = n->siguiente)
        if (n->reserva->getIdReserva() == idReserva) return n;
    return nullptr;
}

// HU5 - Eliminar reserva cancelada
bool TablaHash::eliminar(const string& id) {
    int idx = hash(id);
    if (idx < 0) return false;
    NodoHash* anterior = nullptr;
    NodoHash* actual = regHash[idx];
    while (actual) {
        if (actual->reserva->getIdReserva() == id) {
            // Si es el primero, cambia la cabeza; si no, se enlaza el anterior.
            if (anterior == nullptr) regHash[idx] = actual->siguiente;
            else anterior->siguiente = actual->siguiente;
            delete actual;
            --total;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

TablaHash::~TablaHash() {
    for (auto &cabeza : regHash) while (cabeza) {
        NodoHash* siguiente = cabeza->siguiente;
        delete cabeza;
        cabeza = siguiente;
    }
}
