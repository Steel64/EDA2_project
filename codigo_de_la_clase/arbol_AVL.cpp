#include <iostream>
using namespace std;

struct Nodo {
    int valor;
    int altura;          // altura de una hoja = 1
    Nodo* izq;
    Nodo* der;

    Nodo(int v) {
        valor = v;
        altura = 1;
        izq = nullptr;
        der = nullptr;
    }
};

class ArbolAVL {
private:
    Nodo* raiz;

    // ================= ALTURA Y FACTOR DE BALANCE =================
    int altura(Nodo* x) {
        if (x == nullptr) { return 0; }
        return x->altura;
    }

    void actualizarAltura(Nodo* x) {
        int hi = altura(x->izq);
        int hd = altura(x->der);
        if (hi > hd) { x->altura = 1 + hi; }
        else         { x->altura = 1 + hd; }
    }

    // factor de balance = altura izquierda - altura derecha
    //   > 1  -> pesa a la izquierda
    //   < -1 -> pesa a la derecha
    int factor(Nodo* x) {
        if (x == nullptr) { return 0; }
        return altura(x->izq) - altura(x->der);
    }


    // ================= LAS 2 ROTACIONES BASICAS =================
    Nodo* rotarDerecha(Nodo* y) {
        Nodo* x = y->izq;
        y->izq = x->der;          
        x->der = y;               
        actualizarAltura(y);      
        actualizarAltura(x);      
        return x;                 
    }

    Nodo* rotarIzquierda(Nodo* x) {
        Nodo* y = x->der;
        x->der = y->izq;          
        y->izq = x;               
        actualizarAltura(x);      
        actualizarAltura(y);      
        return y;
    }


    // ================= LOS 4 CASOS DE DESBALANCE =================
    Nodo* casoLL(Nodo* x) {
        return rotarDerecha(x);
    }

    Nodo* casoRR(Nodo* x) {
        return rotarIzquierda(x);
    }

    Nodo* casoLR(Nodo* x) {
        x->izq = rotarIzquierda(x->izq);
        return rotarDerecha(x);
    }

    Nodo* casoRL(Nodo* x) {
        x->der = rotarDerecha(x->der);
        return rotarIzquierda(x);
    }

    // ===== CASOS ======
    Nodo* balancear(Nodo* x) {
        actualizarAltura(x);
        int fb = factor(x);

        if (fb > 1) {                                   // Izquierda
            if (factor(x->izq) >= 0) {
                return casoLL(x);
            } else {
                return casoLR(x);
            }
        }

        if (fb < -1) {                                  // Derecha
            if (factor(x->der) <= 0) {
                return casoRR(x);
            } else {
                return casoRL(x);
            }
        }

        return x;                                       
    }

    //  INSERTAR (recursivo) 
    Nodo* insertarRec(Nodo* x, int valor) {
        if (x == nullptr) {
            return new Nodo(valor);
        }
        if (valor < x->valor) {
            x->izq = insertarRec(x->izq, valor);
        } else if (valor > x->valor) {
            x->der = insertarRec(x->der, valor);
        } else {
            return x;                                   
        }
        return balancear(x);                            
    }

    // ELIMINAR
    Nodo* minimoNodo(Nodo* x) {
        while (x->izq != nullptr) { x = x->izq; }
        return x;
    }

    Nodo* eliminarRec(Nodo* x, int valor) {
        if (x == nullptr) { return nullptr; }

        if (valor < x->valor)      { x->izq = eliminarRec(x->izq, valor); }
        else if (valor > x->valor) { x->der = eliminarRec(x->der, valor); }
        else {
            if (x->izq == nullptr || x->der == nullptr) {   // 0 o 1 hijo
                Nodo* hijo = x->izq;
                if (hijo == nullptr) { hijo = x->der; }
                delete x;
                return hijo;
            }
            Nodo* sucesor = minimoNodo(x->der);             // 2 hijos
            x->valor = sucesor->valor;
            x->der = eliminarRec(x->der, sucesor->valor);
        }
        return balancear(x);                                // rebalancea en cada nivel
    }

    

    //  MOSTRAR
    void inordenRec(Nodo* x) {
        if (x == nullptr) { return; }
        inordenRec(x->izq);
        cout << x->valor << " ";
        inordenRec(x->der);
    }

    
    void mostrarRec(Nodo* x, int nivel) {
        if (x == nullptr) { return; }
        mostrarRec(x->der, nivel + 1);
        for (int i = 0; i < nivel; i++) { cout << "     "; }

        cout << x->valor << " (fb=" << factor(x) << ")" << endl;
        mostrarRec(x->izq, nivel + 1);
    }

    void liberar(Nodo* x) {
        if (x == nullptr) { return; }
        liberar(x->izq);
        liberar(x->der);
        delete x;
    }

public:
    ArbolAVL() {
        raiz = nullptr;
    }

    ~ArbolAVL() {
        liberar(raiz);
    }

    void insertar(int valor) {
        raiz = insertarRec(raiz, valor);
    }
    void eliminar(int valor) {
        raiz = eliminarRec(raiz, valor);
    }

    void inorden() {
        inordenRec(raiz);
        cout << endl;
    }

    void mostrar() {
        if (raiz == nullptr) { cout << "(arbol vacio)" << endl; return; }
        mostrarRec(raiz, 0);
    }
};

// inserta una lista de valores mostrando el árbol final
void probar(int valores[], int cantidad) {
    ArbolAVL arbol;

    cout << "Insertando: ";
    for (int i = 0; i < cantidad; i++) { cout << valores[i] << " "; }
    cout << endl;

    for (int i = 0; i < cantidad; i++) {
        arbol.insertar(valores[i]);
    }

    cout << "Arbol resultante:" << endl;
    arbol.mostrar();
    cout << "Inorden: ";
    arbol.inorden();
    cout << endl;

    cout << "ELIMINAR 30: "<<endl;
    arbol.eliminar(30);
    arbol.mostrar();
    //arbol.eliminar(50);
    //arbol.mostrar();
    //arbol.eliminar(40);
    //arbol.mostrar();
}

int main() {
    //int ll[3] = {30, 20, 10};
    int ll[7] = {10, 20, 30, 40, 50, 25,35};
    //int rr[3] = {10, 20, 30};
    //int lr[3] = {30, 10, 20};
    //int rl[3] = {10, 30, 20};
    //int mix[6] = {10, 20, 30, 40, 50, 25};

    probar( ll, 7);
    
    //probar( rr, 3);
    //probar( lr, 3);
    //probar( rl, 3);
    //probar( mix, 6);

    return 0;
}