#include <iostream>
#include <string>
#include "../base_datos/reservas.h"
#include "../base_datos/vuelos.h"
using namespace std;

const int CAPACIDAD = 300;
extern int TOTAL_ESPERAS;

struct NodoHeap {
    string codVuelo;
    Reserva* heap[CAPACIDAD];
    int tam;
};

class TablaHeap {
    private:
        NodoHeap* regHeap[MAX_VUELOS];
        int padre(int i);
        int izquierdo(int i);
        int derecho(int i);

    public:
        TablaHeap();
        void prepararRegistros();
        NodoHeap* buscarHeap(string codRegHeap);
        void intercambiar(Reserva* heap[], int i, int j);
        int categorizador(Reserva* heap[], int i);
        void heapifyUp(Reserva* heap[], int i);
        bool insertar(Reserva* dataHeap, string codRegHeap);
        void mostrar(string codRegHeap);
};