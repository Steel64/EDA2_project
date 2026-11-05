#include<iostream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Reserva {
    private:
        string IdReserva;
        string Dni;
        string Pasajero;
        string CodigoVuelo;
        string Origen;
        string Destino;
        year_month_day Fecha;
        hh_mm_ss<minutes> Hora;
        string Clase;
        string Categoria;
        float Precio;
        string Estado;
    public:
        Reserva();
        Reserva(string idReserva,string dni,string pasajero,string codigoVuelo,string origen,string destino,year_month_day fecha,hh_mm_ss<minutes> hora,string clase,string categoria,float precio,string estado);
        // Setters
        void setIdReserva(string idReserva);
        void setDni(string dni);
        void setPasajero(string pasajero);
        void setCodigoVuelo(string codigoVuelo);
        void setOrigen(string origen);
        void setDestino(string destino);
        void setFecha(year_month_day fecha);
        void setHora(hh_mm_ss<minutes> hora);
        void setClase(string clase);
        void setCategoria(string categoria);
        void setPrecio(float precio);
        void setEstado(string estado);
        // Getters
        string getIdReserva();
        string getDni();
        string getPasajero();
        string getCodigoVuelo();
        string getOrigen();
        string getDestino();
        year_month_day getFecha();
        hh_mm_ss<minutes> getHora();
        string getClase();
        string getCategoria();
        float getPrecio();
        string getEstado();
        //mostrar informacion
        string mostrar();
};