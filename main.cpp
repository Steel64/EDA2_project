#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>
#include "./librerias/registro_hash.h"
#include "./librerias/registro_heap.h"
#include "./librerias/registro_AVL.h"
using namespace std;

TablaHash t_hash;
TablaHeap t_heap;
TablaAVL t_avl;

void cargar_tabla_hash() {
    for (int i=0;i<TOTAL_RESERVAS;i++){
        t_hash.insertar(registroReservas[i]);
    }
}

void cargar_tabla_heap() {
    t_heap.prepararRegistros();
    for (int i=0;i<TOTAL_RESERVAS;i++){
            if (t_heap.buscarHeap(registroReservas[i]->getCodigoVuelo()) != nullptr){
                t_heap.insertar(registroReservas[i], registroReservas[i]->getCodigoVuelo());
            }
    }
}
void cargar_tabla_AVL() {
    for (int i = 0; i < TOTAL_RESERVAS; i++) {
        t_avl.insertar(registroReservas[i]);
    }
}

void continuar(){
    cout << "\n                Presione ENTER para continuar..." << endl;
    cin.get();
    cout << "\n" << endl;
}

void busqueda_por_ID(){
    string ID = "";
    cout << "Ingrese el id de la reserva(ejemplo: RES-000010): ";
    cin >> ID;
    NodoHash* res = t_hash.buscar(ID);
    if (res != nullptr){
        ostringstream precioConDecimales;
        year_month_day fecha = res->reserva->getFecha();
        hh_mm_ss<minutes> hora = res->reserva->getHora();
        precioConDecimales << fixed << setprecision(2) << res->reserva->getPrecio();
        cout << "\nPasajero: " << res->reserva->getPasajero() << endl;
        cout <<  "CodigoVuelo: " << res->reserva->getCodigoVuelo() << endl;
        cout <<  "Origen: " << res->reserva->getOrigen() << endl;
        cout <<  "Destino: " << res->reserva->getDestino() << endl;
        cout <<  "Fecha: " << to_string(unsigned(fecha.day())) << "/" << to_string(unsigned(fecha.month())) + "/" + to_string(int(fecha.year())) << endl;
        cout <<  "Hora: " << to_string(hora.hours().count()) << ":" << (hora.minutes().count() < 10 ? "0" : "") << to_string(hora.minutes().count()) << endl;
        cout <<  "Clase: " << res->reserva->getClase() << endl;
        cout <<  "Categoria: " << res->reserva->getCategoria() << endl;
        cout <<  "Precio: " << "S/." << precioConDecimales.str() << endl;
        cout <<  "Estado: " << res->reserva->getEstado() << endl;
    }
    else{
        cout << "Reserva no encontrada" << endl;
    }
}

void inicializacion(){
    cout << "===============================================================" << endl;
    cout << "                  SISTEMA DE RESERVAS AEREAS" << endl;
    cout << "===============================================================" << endl;
    cout << "\n                 INICIALIZANDO EL SISTEMA" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cargarVuelos();
    cout << "\n  [1/3] Cargando informacion de vuelos........ OK" << endl;
    cargarReservas();
    cout << "  [2/3] Cargando informacion de reservas....... OK" << endl;
    cargar_tabla_hash();
    cargar_tabla_heap();
    cargar_tabla_AVL();
    cout << "  [3/3] Construcción de estructuras de datos..... OK" << endl;
    cout << "\n---------------------------------------------------------------" << endl;
    cout << "                  RESUMEN DE INICIALIZACION" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "\n  Vuelos registrados             : " << to_string(TOTAL_VUELOS) << endl;
    cout << "  Reservas procesadas            : " << to_string(TOTAL_RESERVAS) << endl;
    cout << "  Total en espera                : " << to_string(TOTAL_ESPERAS) << endl;
    cout << "\n---------------------------------------------------------------" << endl;
    cout << "  Estado del sistema: LISTO PARA OPERAR" << endl;
    cout << "===============================================================" << endl;
    char tecla;
    do{
        cout << "\n                Presione ENTER para continuar..." << endl;
        cin.get(tecla);
    } while(tecla != '\n');
}

int main(){
    inicializacion();
    int opcion;
    do {
        cout << "================================================================" << endl;
        cout << "                    MENU PRINCIPAL DEL SISTEMA" << endl; 
        cout << "================================================================" << endl;
        cout << "\n  MODULO DE GESTION DE RESERVAS" << endl;
        cout << "  ----------------------------------------------------------------" << endl;
        cout << "  [1] Registrar nueva reserva" << endl;
        cout << "  [2] Buscar reserva por ID" << endl;
        cout << "  [3] Actualizar estado de reserva" << endl;
        cout << "  [4] Cancelar reserva" << endl;
        cout << "  [5] Eliminar reserva cancelada" << endl;
        cout << "\n  MODULO DE CONSULTAS" << endl;
        cout << "  ----------------------------------------------------------------" << endl;
        cout << "  [6] Consultar reservas cronologicamente" << endl;
        cout << "  [7] Consultar reservas por rango de fechas" << endl;
        cout << "\n  MODULO DE LISTA DE ESPERA" << endl;
        cout << "  ----------------------------------------------------------------" << endl;
        cout << "  [8] Gestionar lista de espera por vuelo" << endl;
        cout << "\n  MODULO DE REPORTES" << endl;
        cout << "  ----------------------------------------------------------------" << endl;
        cout << "  [9] Consultar estadisticas generales" << endl;
        cout << "\n  ----------------------------------------------------------------" << endl;
        cout << "  [0] Salir del sistema" << endl;
        cout << "================================================================" << endl;
        cout << "  Seleccione una opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1:
                break;
            case 2:
                busqueda_por_ID();
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 0:
                cout << "Salir del sistema..."<<endl;
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente." << endl;
        }
        if (opcion != 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continuar();
        }
    } while (opcion != 0);
    return 0;
/*
VISTA PARA REGISTRO NUEVA RESERVA
================================================================
                     REGISTRO DE RESERVA
================================================================

  DATOS DEL PASAJERO
  ----------------------------------------------------------------
  DNI                 :
  Nombre completo     :

  DATOS DEL VUELO
  ----------------------------------------------------------------
  Codigo de vuelo     :
  Origen              :
  Destino             :
  Fecha de salida     :
  Hora de salida      :

  DATOS DE LA RESERVA
  ----------------------------------------------------------------
  Clase               : [1] Economica [2] Premium [3] Ejecutiva
  Categoria           : [1] Premium [2] Frecuente [3] Regular
  Precio              :

  ----------------------------------------------------------------
  [1] Confirmar registro
  [0] Regresar al menu principal

================================================================

GESTIONAR LISTA DE RESERVA
================================================================
                    GESTION DE LISTA DE ESPERA
================================================================

  Codigo de vuelo: LA2041

  VUELO: LA2041
  RUTA : Lima - Cusco

  ----------------------------------------------------------------
  N°   ID RESERVA      PASAJERO          CATEGORIA
  ----------------------------------------------------------------
  1    RES-000025      Ana Torres        Premium
  2    RES-000031      Luis Ramos        Premium
  3    RES-000042      Maria Soto        Frecuente
  4    RES-000055      Pedro Diaz        Regular
  ----------------------------------------------------------------

  SIGUIENTE PASAJERO A ATENDER
  ----------------------------------------------------------------
  Ana Torres - Premium

  ----------------------------------------------------------------
  [1] Consultar otro vuelo
  [0] Regresar al menu principal

================================================================
*/
}
