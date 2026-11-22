#pragma once
#include "registro_hash.h"
#include "registro_AVL.h"
#include "registro_heap.h"

class DatosSistema {
public:
    ~DatosSistema();
};

extern TablaHash t_hash;
extern TablaAVL t_avl;
extern TablaHeap t_heap;

void cargarIndices();
bool dniValido(const string& dni);
bool idReservaValido(const string& id);

// Estas funciones modifican los datos. El menu se encarga de pedirlos y mostrarlos.
Reserva* registrarReserva(const string& id, const string& codigoVuelo, const string& dni,
    const string& pasajero, const string& clase, const string& categoria,
    float precio, string& error);
bool actualizarEstadoReserva(Reserva* reserva);
bool cancelarReserva(Reserva* reserva, Reserva*& promovida);
bool eliminarReservaCancelada(Reserva* reserva);
