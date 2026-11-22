#pragma once
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
    NodoAVL* raiz = nullptr;
    int total = 0;
    int compararClave(long long tiempo, const string& id, NodoAVL* nodo);
    NodoAVL* eliminarR(NodoAVL* x, long long tiempo, const string& id, bool& eliminado);
    void destruir(NodoAVL* x);
    void recopilar(NodoAVL* x, Reserva* resultado[], int limite, int& cantidad);
    void rangoR(NodoAVL* x, long long inicio, long long fin, Reserva* resultado[], int limite, int& cantidad, int& total);

    int altura(NodoAVL* x);
    void actualizarAltura(NodoAVL* x);

    // factor de balance = altura izquierda - altura derecha
    //   > 1  -> pesa a la izquierda
    //   < -1 -> pesa a la derecha
    int factor(NodoAVL* x);

    // ================= LAS 2 ROTACIONES BASICAS =================
    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);

    NodoAVL* balancear(NodoAVL* x);

    long long convertirFechaHora(year_month_day fecha, hh_mm_ss<minutes> hora);

    //  INSERTAR(recursivo)
    NodoAVL* insertarR(NodoAVL* x, Reserva* r, long long tiempo, bool& insertado);

public:

    ~TablaAVL();
    bool insertar(Reserva* reserva);
    bool eliminar(Reserva* reserva);
    int cantidad() const;
    int altura();
    void primeros(Reserva* resultado[], int limite, int& cantidad);
    void rango(year_month_day inicio, year_month_day fin, Reserva* resultado[], int limite, int& cantidad, int& total);

};
