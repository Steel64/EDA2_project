#include "registro_heap.h"

int TOTAL_ESPERAS=0;

int TablaHeap::padre(int posicion) {
    return (posicion-1)/2;
}

TablaHeap::TablaHeap() {
    for (int i=0; i<MAX_VUELOS; ++i) regHeap[i]=nullptr;
}

TablaHeap::~TablaHeap() {
    // Las reservas se comparten con hash y AVL: aqui solo se borran los heaps.
    for (int i=0; i<MAX_VUELOS; ++i) delete regHeap[i];
}

void TablaHeap::prepararRegistros() {
    for (int i=0; i<TOTAL_VUELOS; i++) {
        if (regHeap[i]==nullptr) {
            regHeap[i]=new NodoHeap{};
            regHeap[i]->codVuelo=registroVuelos[i]->getCodigoVuelo();
        }
    }
}

NodoHeap* TablaHeap::buscarHeap(string codigo) {
    for (int i=0; i<TOTAL_VUELOS; ++i) {
        if (regHeap[i]!=nullptr && regHeap[i]->codVuelo==codigo) return regHeap[i];
    }
    return nullptr;
}

void TablaHeap::intercambiar(EntradaHeap heap[], int a, int b) {
    // Se mueve la casilla completa para conservar el numero de llegada.
    EntradaHeap temporal=heap[a];
    heap[a]=heap[b];
    heap[b]=temporal;
}

int TablaHeap::categorizador(Reserva* reserva) {
    string categoria=reserva->getCategoria();
    if (categoria=="Premium") return 3;
    if (categoria=="Frecuente") return 2;
    if (categoria=="Regular") return 1;
    return 0;
}

bool TablaHeap::mayor(const EntradaHeap& a, const EntradaHeap& b) {
    int categoriaA=categorizador(a.reserva);
    int categoriaB=categorizador(b.reserva);
    if (categoriaA!=categoriaB) return categoriaA>categoriaB;
    // Ante la misma categoria, el numero mas pequeno llego primero.
    return a.llegada<b.llegada;
}

void TablaHeap::heapifyUp(EntradaHeap heap[], int posicion) {
    while (posicion>0) {
        int arriba=padre(posicion);
        if (!mayor(heap[posicion],heap[arriba])) break;
        intercambiar(heap,posicion,arriba);
        posicion=arriba;
    }
}

void TablaHeap::heapifyDown(NodoHeap* heap, int posicion) {
    while (true) {
        int mejor=posicion;
        int izq=2*posicion+1;
        int der=2*posicion+2;
        // Elegir el de mayor prioridad entre el padre y sus dos hijos.
        if (izq<heap->tam && mayor(heap->heap[izq],heap->heap[mejor])) mejor=izq;
        if (der<heap->tam && mayor(heap->heap[der],heap->heap[mejor])) mejor=der;
        if (mejor==posicion) break;
        intercambiar(heap->heap,posicion,mejor);
        posicion=mejor;
    }
}

bool TablaHeap::insertar(Reserva* reserva, string codigo) {
    if (!reserva || reserva->getCodigoVuelo()!=codigo) return false;
    NodoHeap* heap=buscarHeap(codigo);
    if (!heap || heap->tam>=CAPACIDAD || reserva->getEstado()!="EnEspera") return false;
    for (int i=0; i<heap->tam; ++i) {
        if (heap->heap[i].reserva==reserva) return false;
    }

    // Agregar al final y subir hasta que el padre tenga mas prioridad.
    int posicion=heap->tam;
    heap->heap[posicion].reserva=reserva;
    heap->heap[posicion].llegada=siguiente;
    ++siguiente;
    ++heap->tam;
    heapifyUp(heap->heap,posicion);
    ++TOTAL_ESPERAS;
    return true;
}

bool TablaHeap::eliminar(Reserva* reserva) {
    if (!reserva) return false;
    NodoHeap* heap=buscarHeap(reserva->getCodigoVuelo());
    if (!heap) return false;
    int posicion=0;
    while (posicion<heap->tam && heap->heap[posicion].reserva!=reserva) ++posicion;
    if (posicion==heap->tam) return false;

    // La ultima casilla ocupa el hueco. Solo ese camino puede quedar desordenado.
    --heap->tam;
    heap->heap[posicion]=heap->heap[heap->tam];
    --TOTAL_ESPERAS;
    if (posicion<heap->tam) {
        if (posicion>0 && mayor(heap->heap[posicion],heap->heap[padre(posicion)])) {
            heapifyUp(heap->heap,posicion);
        } else {
            heapifyDown(heap,posicion);
        }
    }
    return true;
}

Reserva* TablaHeap::extraerMax(const string& codigo) {
    NodoHeap* heap=buscarHeap(codigo);
    if (!heap || heap->tam==0) return nullptr;
    Reserva* primera=heap->heap[0].reserva;
    eliminar(primera);
    return primera;
}

void TablaHeap::ordenados(const string& codigo, Reserva* resultado[], int& cantidad) {
    cantidad=0;
    NodoHeap* original=buscarHeap(codigo);
    if (!original) return;
    NodoHeap copia=*original;

    // Extraer la raiz de la copia repetidamente deja el resultado en prioridad.
    // No se cambia el heap original ni el contador global de esperas.
    while (copia.tam>0) {
        resultado[cantidad]=copia.heap[0].reserva;
        ++cantidad;
        --copia.tam;
        copia.heap[0]=copia.heap[copia.tam];
        if (copia.tam>0) heapifyDown(&copia,0);
    }
}
