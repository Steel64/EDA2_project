#include "vuelo.h"

Vuelo::Vuelo(){
    CodigoVuelo = "";
    Origen = "";
    Destino = "";
    Fecha = year_month_day{year{1900},month{1},day{1}};
    Hora = hh_mm_ss<minutes>{hours{12}+minutes{10}};
    Capacidad = 0;
}
Vuelo::Vuelo(string codigoVuelo,string origen,string destino,year_month_day fecha,hh_mm_ss<minutes> hora,int capacidad){
    CodigoVuelo = codigoVuelo;
    Origen = origen;
    Destino = destino;
    Fecha = fecha;
    Hora = hora;
    Capacidad = capacidad;
}
// Setters
void Vuelo::setCodigoVuelo(string codigoVuelo) { CodigoVuelo = codigoVuelo; }
void Vuelo::setOrigen(string origen) { Origen = origen; }
void Vuelo::setDestino(string destino) { Destino = destino; }
void Vuelo::setFecha(year_month_day fecha) { Fecha = fecha; }
void Vuelo::setHora(hh_mm_ss<minutes> hora) { Hora = hora; }
void Vuelo::setCapacidad(int capacidad) { Capacidad = capacidad; }
// Getters
string Vuelo::getCodigoVuelo() { return CodigoVuelo; }
string Vuelo::getOrigen() { return Origen; }
string Vuelo::getDestino() { return Destino; }
year_month_day Vuelo::getFecha() { return Fecha; }
hh_mm_ss<minutes> Vuelo::getHora() { return Hora; }
int Vuelo::getCapacidad() { return Capacidad; }
//mostrar informacion
string Vuelo::mostrar() {
    return "CodigoVuelo: " + CodigoVuelo
            + ", Origen: " + Origen
            + ", Destino: " + Destino
            + ", Fecha: " + to_string(int(Fecha.year())) + "-" + to_string(unsigned(Fecha.month())) + "-" + to_string(unsigned(Fecha.day()))
            + ", Hora: " + to_string(Hora.hours().count()) + ":" + (Hora.minutes().count() < 10 ? "0" : "") + to_string(Hora.minutes().count())
            + ", Capacidad: " + to_string(Capacidad);
}