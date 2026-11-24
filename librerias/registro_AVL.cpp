#include "registro_AVL.h"

int TablaAVL::altura(NodoAVL* x) {
    if (x == nullptr) { return 0; }
    return x->altura;
}

void TablaAVL::actualizarAltura(NodoAVL* x) {
    int hi = altura(x->izq);
    int hd = altura(x->der);
    if (hi > hd) { x->altura = 1 + hi; }
    else         { x->altura = 1 + hd; }
}
int TablaAVL::factor(NodoAVL* x) {
    if (x == nullptr) { return 0; }
    return altura(x->izq) - altura(x->der);
}

NodoAVL* TablaAVL::rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izq;
    y->izq = x->der;
    x->der = y;
    actualizarAltura(y); 
    actualizarAltura(x);
    return x;          
}

NodoAVL* TablaAVL::rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->der;
    x->der = y->izq;          
    y->izq = x;               
    actualizarAltura(x);      
    actualizarAltura(y);      
    return y;
}

NodoAVL* TablaAVL::balancear(NodoAVL* nodo) {
    actualizarAltura(nodo);
    int balance=factor(nodo);
    if (balance>1) {
        // LR necesita primero girar el hijo; LL solo gira el nodo.
        if (factor(nodo->izq)<0) nodo->izq=rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }
    if (balance<-1) {
        // RL necesita primero girar el hijo; RR solo gira el nodo.
        if (factor(nodo->der)>0) nodo->der=rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }
    return nodo;
}

long long TablaAVL::convertirFechaHora(year_month_day fecha, hh_mm_ss<minutes> hora) {
    return duration_cast<minutes>(sys_days{fecha}.time_since_epoch()+hora.to_duration()).count();
}

// Resultado negativo: va a la izquierda; positivo: a la derecha; cero: misma clave.
int TablaAVL::compararClave(long long tiempo, const string& id, NodoAVL* nodo) {
    if (tiempo<nodo->tiempoTranscurrido) return -1;
    if (tiempo>nodo->tiempoTranscurrido) return 1;
    if (id<nodo->reserva->getIdReserva()) return -1;
    if (id>nodo->reserva->getIdReserva()) return 1;
    return 0;
}

NodoAVL* TablaAVL::insertarR(NodoAVL* nodo, Reserva* reserva, long long tiempo, bool& insertado) {
    if (!nodo) {
        insertado=true;
        return new NodoAVL(reserva,tiempo);
    }
    int comparacion=compararClave(tiempo,reserva->getIdReserva(),nodo);
    if (comparacion<0) {
        nodo->izq=insertarR(nodo->izq,reserva,tiempo,insertado);
    } else if (comparacion>0) {
        nodo->der=insertarR(nodo->der,reserva,tiempo,insertado);
    } else {
        return nodo; // Una clave repetida no agrega un nodo.
    }
    return balancear(nodo);
}

bool TablaAVL::insertar(Reserva* reserva) {
    if (!reserva) return false;
    long long tiempo=convertirFechaHora(reserva->getFecha(),reserva->getHora());
    bool insertado=false;
    raiz=insertarR(raiz,reserva,tiempo,insertado);
    // El contador cambia una vez, al terminar toda la recursion.
    if (insertado) ++total;
    return insertado;
}

NodoAVL* TablaAVL::eliminarR(NodoAVL* nodo, long long tiempo, const string& id, bool& eliminado) {
    if (!nodo) return nullptr;
    int comparacion=compararClave(tiempo,id,nodo);
    if (comparacion<0) {
        nodo->izq=eliminarR(nodo->izq,tiempo,id,eliminado);
    } else if (comparacion>0) {
        nodo->der=eliminarR(nodo->der,tiempo,id,eliminado);
    } else {
        eliminado=true;
        // Caso 1: sin hijo izquierdo. El derecho puede ser tambien nulo.
        if (!nodo->izq) {
            NodoAVL* hijo=nodo->der;
            delete nodo;
            return hijo;
        }
        // Caso 2: solo tiene hijo izquierdo.
        if (!nodo->der) {
            NodoAVL* hijo=nodo->izq;
            delete nodo;
            return hijo;
        }
        // Caso 3: dos hijos. Sustituir por el menor del subarbol derecho.
        NodoAVL* sucesor=nodo->der;
        while (sucesor->izq) sucesor=sucesor->izq;
        nodo->reserva=sucesor->reserva;
        nodo->tiempoTranscurrido=sucesor->tiempoTranscurrido;
        nodo->der=eliminarR(nodo->der,sucesor->tiempoTranscurrido,
                           sucesor->reserva->getIdReserva(),eliminado);
    }
    // Al volver de la recursion se actualizan alturas y se corrige el balance.
    return balancear(nodo);
}

// HU5 - Eliminar reserva cancelada
bool TablaAVL::eliminar(Reserva* reserva) {
    if (!reserva) return false;
    long long tiempo=convertirFechaHora(reserva->getFecha(),reserva->getHora());
    bool eliminado=false;
    raiz=eliminarR(raiz,tiempo,reserva->getIdReserva(),eliminado);
    if (eliminado) --total;
    return eliminado;
}

void TablaAVL::destruir(NodoAVL* nodo) {
    if (!nodo) return;
    destruir(nodo->izq);
    destruir(nodo->der);
    delete nodo; // La reserva compartida se libera en la gestion de reservas.
}

TablaAVL::~TablaAVL() {
    destruir(raiz);
}

int TablaAVL::cantidad() const {
    return total;
}

int TablaAVL::altura() {
    return altura(raiz);
}

void TablaAVL::recopilar(NodoAVL* nodo, Reserva* resultado[], int limite, int& cantidad) {
    if (!nodo || cantidad>=limite) return;
    recopilar(nodo->izq,resultado,limite,cantidad);
    if (cantidad<limite) {
        resultado[cantidad]=nodo->reserva;
        ++cantidad;
    }
    recopilar(nodo->der,resultado,limite,cantidad);
}

// HU6 - Listado cronologico
void TablaAVL::primeros(Reserva* resultado[], int limite, int& cantidad) {
    cantidad=0;
    recopilar(raiz,resultado,limite,cantidad);
}

void TablaAVL::rangoR(NodoAVL* nodo, long long inicio, long long fin,
                     Reserva* resultado[], int limite, int& cantidad, int& encontrados) {
    if (!nodo) return;
    // Si el nodo es anterior al inicio, toda su izquierda queda fuera del rango.
    if (nodo->tiempoTranscurrido>=inicio) {
        rangoR(nodo->izq,inicio,fin,resultado,limite,cantidad,encontrados);
    }
    if (nodo->tiempoTranscurrido>=inicio && nodo->tiempoTranscurrido<=fin) {
        ++encontrados;
        if (cantidad<limite) {
            resultado[cantidad]=nodo->reserva;
            ++cantidad;
        }
    }
    // Si el nodo es posterior al final, toda su derecha queda fuera del rango.
    if (nodo->tiempoTranscurrido<=fin) {
        rangoR(nodo->der,inicio,fin,resultado,limite,cantidad,encontrados);
    }
}

// HU7 - Consulta por rango de fechas
void TablaAVL::rango(year_month_day inicio, year_month_day fin,
                    Reserva* resultado[], int limite, int& cantidad, int& encontrados) {
    long long primerMinuto=convertirFechaHora(inicio,hh_mm_ss<minutes>{minutes{0}});
    long long ultimoMinuto=convertirFechaHora(fin,hh_mm_ss<minutes>{minutes{1439}});
    cantidad=0;
    encontrados=0;
    rangoR(raiz,primerMinuto,ultimoMinuto,resultado,limite,cantidad,encontrados);
}
