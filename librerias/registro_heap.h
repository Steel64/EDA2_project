#pragma once
#include <iostream>
#include <string>
#include "../base_datos/reservas.h"
#include "../base_datos/vuelos.h"
using namespace std;

const int CAPACIDAD = 300;
extern int TOTAL_ESPERAS;

struct EntradaHeap {
    Reserva* reserva;
    unsigned long long llegada;
};

struct NodoHeap {
    string codVuelo;
    EntradaHeap heap[CAPACIDAD];
    int tam;
};

class TablaHeap {
    private:
        NodoHeap* regHeap[MAX_VUELOS];
        unsigned long long siguiente = 0;
        bool mayor(const EntradaHeap& a, const EntradaHeap& b);
        void heapifyDown(NodoHeap* h, int i);
        int padre(int i);

    public:
        TablaHeap();
        ~TablaHeap();
        Reserva* extraerMax(const string& codigo);
        bool eliminar(Reserva* reserva);
        void ordenados(const string& codigo, Reserva* resultado[], int& cantidad);
        void prepararRegistros();
        NodoHeap* buscarHeap(string codRegHeap);
        void intercambiar(EntradaHeap heap[], int i, int j);
        int categorizador(Reserva* reserva);
        void heapifyUp(EntradaHeap heap[], int i);
        bool insertar(Reserva* dataHeap, string codRegHeap);
};
