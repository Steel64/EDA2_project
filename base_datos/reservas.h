#include "reserva.h"
#include <cstring>
using namespace std;

const int MAX_RESERVAS = 11000;
extern int TOTAL_RESERVAS;
extern Reserva * registroReservas[MAX_RESERVAS];

void insertarReserva(string idReserva,string dni,string pasajero,string codigoVuelo,string origen,string destino,year_month_day fecha,hh_mm_ss<minutes> hora,string clase,string categoria,float precio,string estado);
void cargarReservas();