#include "vuelo.h"
#include <cstring>
using namespace std;

const int MAX_VUELOS = 60;
extern int TOTAL_VUELOS;
extern Vuelo * registroVuelos[MAX_VUELOS];

void insertarVuelos(string codigoVuelo,string origen,string destino,year_month_day fecha,hh_mm_ss<minutes> hora,int capacidad);
void cargarVuelos();