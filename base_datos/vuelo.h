#include<iostream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Vuelo {
    private:
        string CodigoVuelo;
        string Origen;
        string Destino;
        year_month_day Fecha;
        hh_mm_ss<minutes> Hora;
        int Capacidad;
    public:
        Vuelo();
        Vuelo(string codigoVuelo,string origen,string destino,year_month_day fecha,hh_mm_ss<minutes> hora,int capacidad);
        // Setters
        void setCodigoVuelo(string codigoVuelo);
        void setOrigen(string origen);
        void setDestino(string destino);
        void setFecha(year_month_day fecha);
        void setHora(hh_mm_ss<minutes> hora);
        void setCapacidad(int capacidad);
        // Getters
        string getCodigoVuelo();
        string getOrigen();
        string getDestino();
        year_month_day getFecha();
        hh_mm_ss<minutes> getHora();
        int getCapacidad();
        //mostrar informacion
        string mostrar();
};
