#include "gestion_reservas.h"
#include <cmath>

// Se crea primero: su destructor libera los datos despues de destruir los indices.
DatosSistema datosSistema;
TablaHash t_hash;
TablaAVL t_avl;
TablaHeap t_heap;

DatosSistema::~DatosSistema() {
    for (int i=0; i<TOTAL_RESERVAS; ++i) {
        delete registroReservas[i];
        registroReservas[i]=nullptr;
    }
    for (int i=0; i<TOTAL_VUELOS; ++i) {
        delete registroVuelos[i];
        registroVuelos[i]=nullptr;
    }
    TOTAL_RESERVAS=0;
    TOTAL_VUELOS=0;
}

void cargarIndices() {
    t_heap.prepararRegistros();
    for (int i=0; i<TOTAL_RESERVAS; ++i) {
        Reserva* reserva=registroReservas[i];
        t_hash.insertar(reserva);
        t_avl.insertar(reserva);
        if (reserva->getEstado()=="EnEspera") {
            if (!t_heap.insertar(reserva,reserva->getCodigoVuelo())) {
                cerr << "No se pudo cargar en espera: " << reserva->getIdReserva() << '\n';
            }
        }
    }
}

bool dniValido(const string& dni) {
    if (dni.size()!=8) return false;
    for (int i=0; i<8; ++i) {
        if (dni[i]<'0' || dni[i]>'9') return false;
    }
    return true;
}

bool idReservaValido(const string& id) {
    if (id.size()!=10 || id.substr(0,4)!="RES-") return false;
    for (int i=4; i<10; ++i) {
        if (id[i]<'0' || id[i]>'9') return false;
    }
    return id!="RES-000000";
}

// HU1 - Registrar nueva reserva
Reserva* registrarReserva(const string& id, const string& codigoVuelo, const string& dni,
    const string& pasajero, const string& clase, const string& categoria,
    float precio, string& error) {
    error="";
    Vuelo* vuelo=buscarVuelo(codigoVuelo);
    if (!idReservaValido(id)) error="ID invalido: use RES- y seis digitos positivos.";
    else if (t_hash.buscar(id)) error="Ya existe una reserva con ese ID.";
    else if (!vuelo) error="Vuelo no encontrado.";
    else if (TOTAL_RESERVAS>=MAX_RESERVAS) error="Se alcanzo el limite de reservas.";
    else if (!dniValido(dni)) error="DNI invalido: debe contener exactamente 8 digitos.";
    else if (pasajero.empty()) error="El nombre es obligatorio.";
    else if (clase!="Economica" && clase!="Premium" && clase!="Ejecutiva") error="Clase invalida.";
    else if (categoria!="Premium" && categoria!="Frecuente" && categoria!="Regular") error="Categoria invalida.";
    else if (!isfinite(precio) || precio<0) error="Precio invalido.";
    if (!error.empty()) return nullptr;

    string estado="Confirmada";
    if (contarAsientosOcupados(codigoVuelo)>=vuelo->getCapacidad()) {
        NodoHeap* espera=t_heap.buscarHeap(codigoVuelo);
        if (!espera || espera->tam>=CAPACIDAD) {
            error="Lista de espera llena o no disponible.";
            return nullptr;
        }
        estado="EnEspera";
    }

    insertarReserva(id,dni,pasajero,codigoVuelo,vuelo->getOrigen(),vuelo->getDestino(),
        vuelo->getFecha(),vuelo->getHora(),clase,categoria,precio,estado);
    Reserva* nueva=registroReservas[TOTAL_RESERVAS-1];
    t_hash.insertar(nueva);
    t_avl.insertar(nueva);
    if (estado=="EnEspera") t_heap.insertar(nueva,codigoVuelo);
    return nueva;
}

// HU3 - Actualizar estado de reserva
bool actualizarEstadoReserva(Reserva* reserva) {
    if (!reserva) return false;
    if (reserva->getEstado()=="Pendiente") {
        reserva->setEstado("Confirmada");
    } else if (reserva->getEstado()=="Confirmada") {
        reserva->setEstado("CheckIn");
    } else {
        return false;
    }
    return true;
}

// HU4 - Cancelar reserva
bool cancelarReserva(Reserva* reserva, Reserva*& promovida) {
    promovida=nullptr;
    if (!reserva || reserva->getEstado()=="Cancelada") return false;
    string estadoAnterior=reserva->getEstado();
    if (estadoAnterior=="EnEspera") t_heap.eliminar(reserva);
    reserva->setEstado("Cancelada");

    bool ocupabaAsiento=estadoAnterior=="Pendiente" || estadoAnterior=="Confirmada"
                       || estadoAnterior=="CheckIn";
    if (ocupabaAsiento) {
        promovida=t_heap.extraerMax(reserva->getCodigoVuelo());
        if (promovida) promovida->setEstado("Confirmada");
    }
    return true;
}

// HU5 - Eliminar reserva cancelada
bool eliminarReservaCancelada(Reserva* reserva) {
    if (!reserva || reserva->getEstado()!="Cancelada") return false;
    int posicion=0;
    while (posicion<TOTAL_RESERVAS && registroReservas[posicion]!=reserva) ++posicion;
    if (posicion==TOTAL_RESERVAS) return false;

    // Los indices solo poseen sus nodos; la reserva se libera una sola vez aqui.
    t_hash.eliminar(reserva->getIdReserva());
    t_avl.eliminar(reserva);
    for (int i=posicion+1; i<TOTAL_RESERVAS; ++i) {
        registroReservas[i-1]=registroReservas[i];
    }
    --TOTAL_RESERVAS;
    registroReservas[TOTAL_RESERVAS]=nullptr;
    delete reserva;
    return true;
}
