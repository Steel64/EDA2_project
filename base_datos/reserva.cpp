#include "reserva.h"
#include <iomanip>
#include <sstream>

Reserva::Reserva(){
    IdReserva = "";
    Dni = "";
    Pasajero = "";
    CodigoVuelo = "";
    Origen = "";
    Destino = "";
    Fecha = year_month_day{year{1900},month{1},day{1}};
    Hora = hh_mm_ss<minutes>{hours{12}+minutes{10}};
    Clase = "";
    Categoria = "";
    Precio = 0;
    Estado = "";
}
Reserva::Reserva(string idReserva,string dni,string pasajero,string codigoVuelo,string origen,string destino,year_month_day fecha,hh_mm_ss<minutes> hora,string clase,string categoria,float precio,string estado){
    IdReserva = idReserva;
    Dni = dni;
    Pasajero = pasajero;
    CodigoVuelo = codigoVuelo;
    Origen = origen;
    Destino = destino;
    Fecha = fecha;
    Hora = hora;
    Clase = clase;
    Categoria = categoria;
    Precio = precio;
    Estado = estado;
}
// Setters
void Reserva::setIdReserva(string idReserva) { IdReserva = idReserva; }
void Reserva::setDni(string dni) { Dni = dni; }
void Reserva::setPasajero(string pasajero) { Pasajero = pasajero; }
void Reserva::setCodigoVuelo(string codigoVuelo) { CodigoVuelo = codigoVuelo; }
void Reserva::setOrigen(string origen) { Origen = origen; }
void Reserva::setDestino(string destino) { Destino = destino; }
void Reserva::setFecha(year_month_day fecha) { Fecha = fecha; }
void Reserva::setHora(hh_mm_ss<minutes> hora) { Hora = hora; }
void Reserva::setClase(string clase) { Clase = clase; }
void Reserva::setCategoria(string categoria) { Categoria = categoria; }
void Reserva::setPrecio(float precio) { Precio = precio; }
void Reserva::setEstado(string estado) { Estado = estado; }
// Getters
string Reserva::getIdReserva() { return IdReserva; }
string Reserva::getDni() { return Dni; }
string Reserva::getPasajero() { return Pasajero; }
string Reserva::getCodigoVuelo() { return CodigoVuelo; }
string Reserva::getOrigen() { return Origen; }
string Reserva::getDestino() { return Destino; }
year_month_day Reserva::getFecha() { return Fecha; }
hh_mm_ss<minutes> Reserva::getHora() { return Hora; }
string Reserva::getClase() { return Clase; }
string Reserva::getCategoria() { return Categoria; }
float Reserva::getPrecio() { return Precio; }
string Reserva::getEstado() { return Estado; }
//mostrar informacion
string Reserva::mostrar() {
    return "IdReserva: " + IdReserva
            + ", Dni: " + Dni
            + ", Pasajero: " + Pasajero
            + ", CodigoVuelo: " + CodigoVuelo
            + ", Origen: " + Origen
            + ", Destino: " + Destino
            + ", Fecha: " + to_string(int(Fecha.year())) + "-" + to_string(unsigned(Fecha.month())) + "-" + to_string(unsigned(Fecha.day()))
            + ", Hora: " + to_string(Hora.hours().count()) + ":" + (Hora.minutes().count() < 10 ? "0" : "") + to_string(Hora.minutes().count())
            + ", Clase: " + Clase
            + ", Categoria: " + Categoria
            + ", Precio: " + to_string(Precio)
            + ", Estado: " + Estado;
}
