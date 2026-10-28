#include <iostream>
using namespace std;

struct Nodo {
    int dato;
    Nodo* siguiente;
};

class ListaEnlazada {
private:
    Nodo* cabeza;

public:
    ListaEnlazada() {
        cabeza = nullptr;   // lista vacía
    }

    // Insertar un nuevo nodo al INICIO de la lista
    void insertarAlInicio(int valor) {
        Nodo* nuevo = new Nodo();
        nuevo->dato = valor;
        nuevo->siguiente = cabeza;   // el nuevo apunta a lo que era la cabeza
        cabeza = nuevo;              // el nuevo pasa a ser la cabeza
    }

    // Insertar un nuevo nodo al FINAL de la lista
    void insertarAlFinal(int valor) {
        Nodo* nuevo = new Nodo();
        nuevo->dato = valor;
        nuevo->siguiente = nullptr;

        if (cabeza == nullptr) {
            cabeza = nuevo;
            return;
        }
        Nodo* actual = cabeza;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }

    // Buscar un valor en la lista
    bool buscar(int valor) {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            if (actual->dato == valor) return true;
            actual = actual->siguiente;
        }
        return false;
    }

    // Imprimir toda la lista
    void imprimir() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            cout << actual->dato << " -> ";
            actual = actual->siguiente;
        }
        cout << "NULL" << endl;
    }
};

int main() {
    ListaEnlazada lista;
    lista.insertarAlFinal(10);
    lista.insertarAlFinal(20);
    lista.insertarAlFinal(30);
    lista.imprimir();   // 10 -> 20 -> 30 -> NULL

    lista.insertarAlInicio(5);
    lista.imprimir();   // 5 -> 10 -> 20 -> 30 -> NULL

    cout << lista.buscar(20) << endl;  // 1 (true)
    cout << lista.buscar(99) << endl;  // 0 (false)

    return 0;
}