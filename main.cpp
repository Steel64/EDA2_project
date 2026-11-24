#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>
#include "./librerias/gestion_reservas.h"
using namespace std;

void continuar(){
    cout << "\n                Presione ENTER para continuar..." << endl;
    cin.get();
    cout << "\n" << endl;
}

// HU2 - Buscar reserva por ID
void busqueda_por_ID() {
    string id;
    cout << "Ingrese el ID de la reserva (ejemplo: RES-000010): ";
    cin >> id;
    NodoHash* nodo=t_hash.buscar(id);
    if (nodo != nullptr) {
        ostringstream precioConDecimales;
        year_month_day fecha = nodo->reserva->getFecha();
        hh_mm_ss<minutes> hora = nodo->reserva->getHora();
        precioConDecimales << fixed << setprecision(2) << nodo->reserva->getPrecio();
        cout << "\nPasajero: " << nodo->reserva->getPasajero() << endl;
        cout <<  "CodigoVuelo: " << nodo->reserva->getCodigoVuelo() << endl;
        cout <<  "Origen: " << nodo->reserva->getOrigen() << endl;
        cout <<  "Destino: " << nodo->reserva->getDestino() << endl;
        cout <<  "Fecha: " << to_string(unsigned(fecha.day())) << "/" << to_string(unsigned(fecha.month())) + "/" + to_string(int(fecha.year())) << endl;
        cout <<  "Hora: " << to_string(hora.hours().count()) << ":" << (hora.minutes().count() < 10 ? "0" : "") << to_string(hora.minutes().count()) << endl;
        cout <<  "Clase: " << nodo->reserva->getClase() << endl;
        cout <<  "Categoria: " << nodo->reserva->getCategoria() << endl;
        cout <<  "Precio: " << "S/." << precioConDecimales.str() << endl;
        cout <<  "Estado: " << nodo->reserva->getEstado() << endl;
    }
    else cout << "Reserva no encontrada.\n";
}

// HU1 - Registrar nueva reserva
void registrar_reserva() {
    string id;
    cout << "Ingrese el ID de reserva (ejemplo: RES-900001): ";
    cin >> id;
    if (!idReservaValido(id) || t_hash.buscar(id)) {
        cout << "ID invalido o ya registrado.\n";
        return;
    }
    string codigoVuelo;
    cout << "Ingrese el codigo del vuelo: ";
    if (!(cin >> codigoVuelo)) {
        return;
    }

    Vuelo* vuelo = buscarVuelo(codigoVuelo);

    if (vuelo == nullptr) {
        cout << "Vuelo no encontrado.\n";
        return;
    }

    string dni, pasajero, clase, categoria;
    float precio;

    cout << "Ingrese el DNI: ";
    if (!(cin >> dni) || !dniValido(dni)) {
        cout << "DNI invalido: debe contener exactamente 8 digitos.\n";
        return;
    }

    cout << "Ingrese el nombre completo del pasajero: ";
    getline(cin >> ws, pasajero);

    int opcionClase, opcionCategoria;
    const string clases[]={"Economica","Premium","Ejecutiva"};
    const string categorias[]={"Premium","Frecuente","Regular"};
    cout << "Clase: [1] Economica [2] Premium [3] Ejecutiva: ";
    if (!(cin >> opcionClase) || opcionClase < 1 || opcionClase > 3) { cout << "Clase invalida.\n"; return; }
    clase = clases[opcionClase-1];
    cout << "Categoria: [1] Premium [2] Frecuente [3] Regular: ";
    if (!(cin >> opcionCategoria) || opcionCategoria < 1 || opcionCategoria > 3) { cout << "Categoria invalida.\n"; return; }
    categoria = categorias[opcionCategoria-1];

    cout << "Ingrese el precio: ";
    if (!(cin >> precio) || precio < 0) {
        cin.clear();
        cout << "Precio invalido.\n";
        return;
    }

    string error;
    Reserva* nueva=registrarReserva(id,codigoVuelo,dni,pasajero,clase,categoria,precio,error);
    if (nueva) cout << "Reserva registrada. ID: " << nueva->getIdReserva() << " - " << nueva->getEstado() << '\n';
    else cout << error << '\n';
}
void inicializacion(const string& rutaReservas) {
    cout << "\nSISTEMA DE RESERVAS AEREAS - CARGA INICIAL\n";
    cargarVuelos();
    cargarReservas(rutaReservas);
    cargarIndices();
    cout << "Vuelos registrados: " << TOTAL_VUELOS << '\n';
    cout << "Reservas procesadas: " << TOTAL_RESERVAS << '\n';
    cout << "Insertadas en hash: " << t_hash.cantidad() << '\n';
    cout << "Insertadas en AVL: " << t_avl.cantidad() << '\n';
    cout << "Insertadas en los Heaps: " << '\n';
    for (int i=0; i<TOTAL_VUELOS; ++i) {
        NodoHeap* heap=t_heap.buscarHeap(registroVuelos[i]->getCodigoVuelo());
        if (heap && heap->tam>0) cout << "Heap " << heap->codVuelo << ": " << heap->tam << '\n';
    }
    cout << "Total en espera: " << TOTAL_ESPERAS << '\n';
    cout << "Factor de carga hash: " << t_hash.factorCarga() << '\n';
    cout << "Cadena hash mas larga: " << t_hash.cadenaMasLarga() << '\n';
    cout << "Altura del AVL: " << t_avl.altura() << '\n';
    continuar();
}

Reserva* pedirReserva() {
    string id;
    cout << "ID de reserva: ";
    cin >> id;
    NodoHash* n=t_hash.buscar(id);
    if (!n) cout << "Reserva no encontrada.\n";
    return n ? n->reserva : nullptr;
}
// HU3 - Actualizar estado de reserva
void actualizarEstado() {
    Reserva* reserva=pedirReserva();
    if (!reserva) return;
    if (actualizarEstadoReserva(reserva)) {
        cout << "Nuevo estado: " << reserva->getEstado() << '\n';
    } else {
        cout << "Transicion no permitida desde " << reserva->getEstado() << ".\n";
    }
}

// HU4 - Cancelar reserva
void cancelar() {
    Reserva* reserva=pedirReserva();
    if (!reserva) return;
    Reserva* promovida=nullptr;
    if (!cancelarReserva(reserva,promovida)) {
        cout << "Ya estaba cancelada.\n";
        return;
    }
    if (promovida) cout << "Promovida: " << promovida->getIdReserva() << '\n';
    cout << "Reserva cancelada.\n";
}

// HU5 - Eliminar reserva cancelada
void eliminarReserva() {
    Reserva* reserva=pedirReserva();
    if (!reserva) return;
    if (eliminarReservaCancelada(reserva)) cout << "Reserva eliminada.\n";
    else cout << "Solo se eliminan reservas canceladas.\n";
}

void mostrarLista(Reserva* lista[], int cantidad) {
    if (cantidad == 0) { cout << "No hay reservas para mostrar.\n";}
    else {
        for (int i=0; i<cantidad; ++i) cout << lista[i]->getIdReserva() << " - " << lista[i]->mostrar() << '\n';
    }
}
bool leerFecha(year_month_day& fecha) {
    string texto;
    cin >> texto;
    int anio,mes,dia;
    char separador1,separador2;
    istringstream entrada(texto);
    if (!(entrada >> anio >> separador1 >> mes >> separador2 >> dia)) return false;
    if (separador1!='-' || separador2!='-' || entrada.peek()!=EOF) return false;
    if (mes<1 || mes>12 || dia<1 || dia>31) return false;
    fecha=year{anio}/month{static_cast<unsigned>(mes)}/day{static_cast<unsigned>(dia)};
    return fecha.ok();
}
// HU6 - Listado cronologico
void listado() {
    int n;
    cout << "Cantidad [20 o 50]: ";
    if (!(cin >> n) || (n!=20 && n!=50)) { cout << "Cantidad invalida.\n"; return; }
    Reserva* lista[50];
    int cantidad;
    t_avl.primeros(lista,n,cantidad);
    mostrarLista(lista,cantidad);
}
// HU7 - Consulta por rango de fechas
void consultarRango() {
    year_month_day inicio,fin;
    cout << "Fecha inicial (AAAA-MM-DD): ";
    if (!leerFecha(inicio)) { cout << "Fecha invalida.\n"; return; }
    cout << "Fecha final (AAAA-MM-DD): ";
    if (!leerFecha(fin) || sys_days{fin}<sys_days{inicio}) { cout << "Fecha final invalida.\n"; return; }
    Reserva* lista[20];
    int cantidad,total;
    t_avl.rango(inicio,fin,lista,20,cantidad,total);
    cout << "Cantidad: " << total << ". Muestra (hasta 20):\n";
    mostrarLista(lista,cantidad);
}
// HU8 - Lista de espera por vuelo
void espera() {
    string codigo;
    cout << "Codigo de vuelo: ";
    cin >> codigo;
    NodoHeap* heap=t_heap.buscarHeap(codigo);
    if (!heap) { cout << "Vuelo no encontrado.\n"; return; }
    if (heap->tam==0) { cout << "Lista de espera vacia.\n"; return; }
    Reserva* siguiente=heap->heap[0].reserva;
    cout << "Raiz del heap: " << siguiente->getIdReserva() << " - " << siguiente->getPasajero() << '\n';
    Reserva* lista[CAPACIDAD];
    int cantidad;
    t_heap.ordenados(codigo,lista,cantidad);
    for (int i=0; i<cantidad; ++i)
        cout << lista[i]->getIdReserva() << " - " << lista[i]->getPasajero() << " - " << lista[i]->getCategoria() << '\n';
    cout << "Total: " << cantidad << '\n';
}
struct Conteo { string nombre; int cantidad; };
bool contar(Conteo datos[], int& usados, int capacidad, const string& nombre) {
    int i=0;
    while (i<usados && datos[i].nombre!=nombre) ++i;
    if (i==usados) {
        if (usados==capacidad) return false;
        datos[usados].nombre=nombre;
        datos[usados].cantidad=0;
        ++usados;
    }
    ++datos[i].cantidad;
    return true;
}
void imprimirConteos(const char* titulo, const Conteo datos[], int usados) {
    cout << titulo << ":\n";
    for (int i=0; i<usados; ++i) cout << "  " << datos[i].nombre << ": " << datos[i].cantidad << '\n';
}
// HU9 - Estadisticas generales
void estadisticas() {
    Conteo destinos[MAX_VUELOS], clases[3], estados[5];
    bool conteosCompletos=true;
    int totalDestinos=0,totalClases=0,totalEstados=0;
    double ingresos=0,sumaPrecios=0;
    Reserva *menor=nullptr,*mayor=nullptr;
    for (int i=0;i<TOTAL_RESERVAS;++i) {
        Reserva* reserva=registroReservas[i];
        sumaPrecios+=reserva->getPrecio();
        if (!contar(destinos,totalDestinos,MAX_VUELOS,reserva->getDestino())) conteosCompletos=false;
        if (!contar(clases,totalClases,3,reserva->getClase())) conteosCompletos=false;
        if (!contar(estados,totalEstados,5,reserva->getEstado())) conteosCompletos=false;
        if (reserva->getEstado()=="Confirmada" || reserva->getEstado()=="CheckIn") {
            ingresos+=reserva->getPrecio();
        }
        if (!menor || reserva->getPrecio()<menor->getPrecio()) menor=reserva;
        if (!mayor || reserva->getPrecio()>mayor->getPrecio()) mayor=reserva;
    }
    cout << "Total: " << TOTAL_RESERVAS << " | Ingresos: " << fixed << setprecision(2) << ingresos << " | Precio promedio de todas las reservas: " << (TOTAL_RESERVAS ? sumaPrecios/TOTAL_RESERVAS : 0) << '\n';
    if (menor) cout << "Menor precio: " << menor->getPrecio() << " (" << menor->getIdReserva() << ") | Mayor: " << mayor->getPrecio() << " (" << mayor->getIdReserva() << ")\n";
    imprimirConteos("Destinos",destinos,totalDestinos);
    imprimirConteos("Clases",clases,totalClases);
    imprimirConteos("Estados",estados,totalEstados);
    if (!conteosCompletos) cout << "Hay datos fuera de los limites del catalogo.\n";
    NodoHeap* maximo=nullptr;
    for (int i=0;i<TOTAL_VUELOS;++i) {
        NodoHeap* heap=t_heap.buscarHeap(registroVuelos[i]->getCodigoVuelo());
        if (heap && (!maximo || heap->tam>maximo->tam)) maximo=heap;
    }
    if (maximo) cout << "Vuelo con mas espera: " << maximo->codVuelo << " (" << maximo->tam << ")\n";
}

int main(int argc, char* argv[]){
    if (argc > 2 || (argc == 2 && string(argv[1]) != "0" && string(argv[1]) != "1")) {
        cerr << "Uso: " << argv[0] << " [0|1]\n"
             << "  0: cargar csv_s/reservas_pequeno.csv\n"
             << "  1: cargar csv_s/reservas_masivo.csv\n";
        return 1;
    }
    const string rutaReservas = argc == 2 && string(argv[1]) == "1"
        ? "csv_s/reservas_masivo.csv"
        : "csv_s/reservas_pequeno.csv";
    inicializacion(rutaReservas);
    int opcion=0;
    do {
        cout << "\nMENU PRINCIPAL\n";
        cout << "[1] Registrar nueva reserva\n";
        cout << "[2] Buscar reserva por ID\n";
        cout << "[3] Actualizar estado\n";
        cout << "[4] Cancelar reserva\n";
        cout << "[5] Eliminar reserva cancelada\n";
        cout << "[6] Listado cronologico de reservas\n";
        cout << "[7] Consultar por rango de fechas\n";
        cout << "[8] Consultar lista de espera\n";
        cout << "[9] Estadisticas generales\n";
        cout << "[0] Salir\nSeleccione una opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1:
                registrar_reserva();
                break;
            case 2:
                busqueda_por_ID();
                break;
            case 3:
                actualizarEstado();
                break;
            case 4:
                cancelar();
                break;
            case 5:
                eliminarReserva();
                break;
            case 6:
                listado();
                break;
            case 7:
                consultarRango();
                break;
            case 8:
                espera();
                break;
            case 9:
                estadisticas();
                break;
            case 0:
                cout << "Salir del sistema..."<<endl;
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente." << endl;
        }
        if (!cin) { if (cin.eof()) break; cin.clear(); }
        if (opcion != 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continuar();
        }
    } while (opcion != 0);
    return 0;

}
