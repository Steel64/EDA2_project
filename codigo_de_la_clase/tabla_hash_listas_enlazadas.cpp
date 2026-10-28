#include <iostream>
using namespace std;

const int CAPACIDAD = 7;

struct Nodo {
    int valor;
    Nodo* siguiente;
};

class TablaHash {
private:
    Nodo* tabla[CAPACIDAD];

    // Función hash: solo el módulo del valor ingresado
    int hash(int valor) {
        return valor % CAPACIDAD;
    }

public:
    TablaHash() {
        for (int i = 0; i < CAPACIDAD; i++) {
            tabla[i] = nullptr;
        }
    }

    void insertar(int valor) {
        int idx = hash(valor);           // calcula la posición

        Nodo* nuevo = new Nodo();
        nuevo->valor = valor;
        nuevo->siguiente = tabla[idx];   // se agrega al inicio de la lista de ese índice
        tabla[idx] = nuevo;
    }

    bool buscar(int valor) {
        int idx = hash(valor);
        Nodo* actual = tabla[idx];

        while (actual != nullptr) {
            if (actual->valor == valor) return true;
            actual = actual->siguiente;
        }
        return false;
    }

    void imprimir() {
        for (int i = 0; i < CAPACIDAD; i++) {
            cout << "Indice " << i << ": ";
            Nodo* actual = tabla[i];
            while (actual != nullptr) {
                cout << actual->valor << " -> ";
                actual = actual->siguiente;
            }
            cout << "NULL" << endl;
        }
    }
};

int main() {
    TablaHash tabla;

    tabla.insertar(15);
    tabla.insertar(22);
    tabla.insertar(8);
    tabla.insertar(29);
    tabla.imprimir();

    return 0;
}