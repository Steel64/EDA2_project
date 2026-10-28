#include <iostream>
using namespace std;

int hash_[6];
int tam = 6;

void iniciar(){
    for(int i = 0; i<tam ; i++ ){
        hash_[i] = NULL;
    }
}

int funcion_hash(int k){
    int salida = k%6;
    return salida;

}

void insertar_hash(int valor){
    int indice = funcion_hash(valor);
    while(hash_[indice] != NULL){
        indice = (indice + 1)%6;
    }
    hash_[indice] = valor;
}
void mostrar_hash(){
    for(int i=0; i<tam; i++){
        cout<<hash_[i]<<endl;
    }

}

int main(){
    iniciar();
    insertar_hash(7);
    insertar_hash(19);
    mostrar_hash();


}
