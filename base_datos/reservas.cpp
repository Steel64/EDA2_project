#include "reservas.h"
#include "csv.h"
#include <fstream>

int TOTAL_RESERVAS = 0;
Reserva* registroReservas[MAX_RESERVAS];

void insertarReserva(string idReserva,string dni,string pasajero,string codigoVuelo,string origen,string destino,year_month_day fecha,hh_mm_ss<minutes> hora,string clase,string categoria,float precio,string estado){
    registroReservas[TOTAL_RESERVAS] = new Reserva(idReserva,dni,pasajero,codigoVuelo,origen,destino,fecha,hora,clase,categoria,precio,estado);
    TOTAL_RESERVAS++;
}

void cargarReservas(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo) { cerr << "Error al abrir " << ruta << '\n'; return; }
    string linea,campos[12];
    while (TOTAL_RESERVAS<MAX_RESERVAS && getline(archivo,linea)) {
        year_month_day fecha;
        hh_mm_ss<minutes> hora;
        float precio;
        if (!separarCampos(linea,campos,12) || !leerSalida(campos[6],campos[7],fecha,hora)) continue;
        istringstream precioTexto(campos[10]);
        if (!(precioTexto>>precio)) continue;
        insertarReserva(campos[0],campos[1],campos[2],campos[3],campos[4],campos[5],
                        fecha,hora,campos[8],campos[9],precio,campos[11]);
    }
}
int contarAsientosOcupados(string codigoVuelo) {
    int ocupados = 0;

    for (int i = 0; i < TOTAL_RESERVAS; i++) {
        Reserva* reserva = registroReservas[i];

        if (reserva->getCodigoVuelo() == codigoVuelo &&
            (reserva->getEstado() == "Confirmada" || reserva->getEstado() == "Pendiente" || reserva->getEstado() == "CheckIn")) {
            ocupados++;
        }
    }

    return ocupados;
}
