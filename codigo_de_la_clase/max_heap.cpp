#include <iostream>
using namespace std;

const int CAPACIDAD = 12;     // tamaño del arreglo
int n = 0;

int padre(int i){
    return (i - 1) / 2; 
}
int izquierdo(int i) {
    return 2 * i + 1; 
}
int derecho(int i) { 
    return 2 * i + 2; 
}

void intercambiar(int heap[], int i, int j) {
    int temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

// Sube el elemento de la posición i mientras sea mayor que su padre
void heapifyUp(int heap[], int i) {
    while (i > 0 && heap[padre(i)] < heap[i]) {
        intercambiar(heap, padre(i), i);
        i = padre(i);
    }
}

// Baja el elemento de la posición i intercambiándolo con su hijo mayor
void heapifyDown(int heap[], int n, int i) {
    while (true) {
        int mayor = i;
        int izq = izquierdo(i), der = derecho(i);
        if (izq < n && heap[izq] > heap[mayor]) mayor = izq;
        if (der < n && heap[der] > heap[mayor]) mayor = der;
        if (mayor == i) break;                   // ya cumple la propiedad
        intercambiar(heap, i, mayor);
        i = mayor;
    }
}

// Inserta un valor al final y lo sube. Retorna false si el arreglo está lleno.
bool insertar(int heap[], int valor) {
    if (n == CAPACIDAD) return false;
    heap[n] = valor;
    n++;
    heapifyUp(heap, n - 1);
    return true;
}

// Retorna el máximo sin eliminarlo. Retorna false si el heap está vacío.
bool verMax(const int heap[], int& resultado) {
    if (n == 0) return false;
    resultado = heap[0];
    return true;
}

// Elimina y retorna el máximo. Retorna false si el heap está vacío.
bool extraerMax(int heap[], int& resultado) {
    if (n == 0) return false;
    resultado = heap[0];
    heap[0] = heap[n - 1];                       // el último pasa a la raíz
    n--;
    if (n > 0) heapifyDown(heap, n, 0);
    return true;
}


void mostrar(const int heap[]) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << heap[i];
        if (i < n - 1) cout << ", ";
    }
    cout << "]  (n = " << n << ")" << endl;
}

int main() {
    int heap[CAPACIDAD];          // el arreglo
                        // elementos que hay actualmente

    int valores[] = {50, 30, 40, 10, 20, 35, 15};
    for (int i = 0; i < 7; i++)
        insertar(heap, valores[i]);
    mostrar(heap);                        // [50, 30, 40, 10, 20, 35, 15]
    insertar(heap, 45);
    mostrar(heap);                        // [50, 45, 40, 30, 20, 35, 15, 10]

    int max;
    verMax(heap, max);                     // max = 50
    extraerMax(heap, max);                 // max = 50
    mostrar(heap);                        // [45, 30, 40, 10, 20, 35, 15]


    
}