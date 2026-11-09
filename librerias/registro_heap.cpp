#include "registro_heap.h"

int TOTAL_ESPERAS = 0;

int TablaHeap::padre(int i){
    return (i - 1) / 2;
}
int TablaHeap::izquierdo(int i) {
    return 2 * i + 1;
}
int TablaHeap::derecho(int i) { 
    return 2 * i + 2;
}

TablaHeap::TablaHeap(){
    for (int i = 0; i < MAX_VUELOS; i++) {
        regHeap[MAX_VUELOS] = nullptr;
    }
}

void TablaHeap::prepararRegistros(){
    for (int i=0;i<TOTAL_VUELOS;i++){
        NodoHeap* nodo = new NodoHeap();
        nodo->codVuelo = registroVuelos[i]->getCodigoVuelo();
        nodo->tam = 0;
        regHeap[i] = nodo;
    }
}

NodoHeap* TablaHeap::buscarHeap(string codRegHeap){
    for (int i=0;i<MAX_VUELOS;i++){
        if (regHeap[i]->codVuelo == codRegHeap){
            return regHeap[i];
        }
    }
    return nullptr;
}

void TablaHeap::intercambiar(Reserva* heap[], int i, int j) {
    Reserva* temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

int TablaHeap::categorizador(Reserva* heap[], int i) {
    if (heap[i]->getCategoria() == "Premium") {
        return 3 * (300-i);
    }
    else if (heap[i]->getCategoria() == "Frecuente"){
        return 2 * (300-i);
    }
    else if (heap[i]->getCategoria() == "Regular"){
        return 1 * (300-i);
    }
    else {
        return 0;
    }
}

void TablaHeap::heapifyUp(Reserva* heap[], int i) {
    while (i > 0 && categorizador(heap,padre(i)) < categorizador(heap,i)) {
        intercambiar(heap, padre(i), i);
        i = padre(i);
    }
}

bool TablaHeap::insertar(Reserva* dataHeap, string codRegHeap) {
    NodoHeap* x = buscarHeap(codRegHeap);
    if (x != nullptr){
        if (x->tam == CAPACIDAD || dataHeap->getEstado() != "EnEspera") return false;
        x->heap[x->tam] = dataHeap;
        (x->tam)++;
        heapifyUp(x->heap, x->tam - 1);
        TOTAL_ESPERAS++;
        return true;
    }
    return false;
}

void TablaHeap::mostrar(string codRegHeap) {
    NodoHeap* x = buscarHeap(codRegHeap);
    cout << "[";
    for (int i = 0; i < x->tam; i++) {
        cout << x->heap[i]->getCategoria();
        if (i < x->tam - 1) cout << ", ";
    }
    cout << "]  (n = " << x->tam << ")" << endl;
}