#include "vuelos.h"
#include "csv.h"
#include <fstream>

int TOTAL_VUELOS = 0;
Vuelo * registroVuelos[MAX_VUELOS];

void insertarVuelos(string codigoVuelo,string origen,string destino,year_month_day fecha,hh_mm_ss<minutes> hora,int capacidad){
    registroVuelos[TOTAL_VUELOS] = new Vuelo(codigoVuelo,origen,destino,fecha,hora,capacidad);
    TOTAL_VUELOS++;
}

void cargarVuelos() {
    ifstream archivo("csv_s/vuelos.csv");
    if (!archivo) { cerr << "Error al abrir csv_s/vuelos.csv\n"; return; }
    string linea,campos[6];
    while (TOTAL_VUELOS<MAX_VUELOS && getline(archivo,linea)) {
        year_month_day fecha;
        hh_mm_ss<minutes> hora;
        int capacidad;
        if (!separarCampos(linea,campos,6) || !leerSalida(campos[3],campos[4],fecha,hora)) continue;
        istringstream capacidadTexto(campos[5]);
        if (!(capacidadTexto>>capacidad)) continue;
        insertarVuelos(campos[0],campos[1],campos[2],fecha,hora,capacidad);
    }
}
Vuelo* buscarVuelo(string codigoVuelo) {
    for (int i = 0; i < TOTAL_VUELOS; i++) {
        if (registroVuelos[i]->getCodigoVuelo() == codigoVuelo) {
            return registroVuelos[i];
        }
    }

    return nullptr;
}