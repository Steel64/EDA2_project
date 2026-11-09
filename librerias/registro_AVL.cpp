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

NodoAVL* TablaAVL::casoLL(NodoAVL* x) {
    return rotarDerecha(x);
}

NodoAVL* TablaAVL::casoRR(NodoAVL* x) {
    return rotarIzquierda(x);
}

NodoAVL* TablaAVL::casoLR(NodoAVL* x) {
    x->izq = rotarIzquierda(x->izq);
    return rotarDerecha(x);
}

NodoAVL* TablaAVL::casoRL(NodoAVL* x) {
    x->der = rotarDerecha(x->der);
    return rotarIzquierda(x);
}

NodoAVL* TablaAVL::balancear(NodoAVL* x) {
    actualizarAltura(x);
    int fb = factor(x);
    if (fb > 1) { // Izquierda
        if (factor(x->izq) >= 0) {
            return casoLL(x);
        }
        else {
            return casoLR(x);
        }
    }
    if (fb < -1) { // Derecha
        if (factor(x->der) <= 0) {
            return casoRR(x);
        }
        else {
            return casoRL(x);
        }
    }
    return x;                                       
}

void TablaAVL::convertirFechaHora(year_month_day fecha, hh_mm_ss<minutes> hora, long long& tiempo){
    const sys_days diasF(fecha);
    const long long diasH = hora.hours().count() * 60 + hora.minutes().count();
    tiempo = duration_cast<minutes>(diasF.time_since_epoch()).count() + diasH;
}

NodoAVL* TablaAVL::insertarR(NodoAVL* x, Reserva* r, long long tiempo) {
    if (x == nullptr) {
        return new NodoAVL(r,tiempo);
    }
    if (tiempo < x->tiempoTranscurrido) {
        x->izq = insertarR(x->izq, r, tiempo);
    } else {
        x->der = insertarR(x->der, r, tiempo);
    }
    return balancear(x);                            
}

//  MOSTRAR
void TablaAVL::inordenR(NodoAVL* x) {
    if (x == nullptr) { return; }
    inordenR(x->izq);
    cout << x->reserva->getFecha() << " " << x->reserva->getHora() << "\n";
    inordenR(x->der);
}

void TablaAVL::insertar(Reserva* reserva){
    long long tiempo;
    convertirFechaHora(reserva->getFecha(), reserva->getHora(), tiempo);
    raiz = insertarR(raiz, reserva, tiempo);
}

void TablaAVL::inorden() {
    inordenR(raiz);
    cout << endl;
}