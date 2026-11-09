#include <iostream>
#include <string>
#include "../base_datos/reservas.h"
using namespace std;

struct NodoAVL {
    Reserva* reserva;
    long long tiempoTranscurrido;
    int altura;
    NodoAVL* izq;
    NodoAVL* der;

    NodoAVL(Reserva* r, long long tiempoT) {
        reserva = r;
        tiempoTranscurrido = tiempoT;
        altura = 1;
        izq = nullptr;
        der = nullptr;
    }
};

class TablaAVL {
private:
    NodoAVL* raiz;

    int altura(NodoAVL* x);
    void actualizarAltura(NodoAVL* x);

    // factor de balance = altura izquierda - altura derecha
    //   > 1  -> pesa a la izquierda
    //   < -1 -> pesa a la derecha
    int factor(NodoAVL* x);

    // ================= LAS 2 ROTACIONES BASICAS =================
    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);

    // ================= LOS 4 CASOS DE DESBALANCE =================
    NodoAVL* casoLL(NodoAVL* x);
    NodoAVL* casoRR(NodoAVL* x);
    NodoAVL* casoLR(NodoAVL* x);
    NodoAVL* casoRL(NodoAVL* x);

    // ===== CASOS ======
    NodoAVL* balancear(NodoAVL* x);

    void convertirFechaHora(year_month_day fecha, hh_mm_ss<minutes> hora, long long& tiempo);

    //  INSERTAR(recursivo)
    NodoAVL* insertarR(NodoAVL* x, Reserva* r, long long tiempo);

    //  MOSTRAR
    void inordenR(NodoAVL* x);

public:

    void insertar(Reserva* reserva);
    void inorden();

};