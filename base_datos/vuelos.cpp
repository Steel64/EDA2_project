#include "vuelos.h"

int TOTAL_VUELOS = 0;
Vuelo * registroVuelos[MAX_VUELOS];

void insertarVuelos(string codigoVuelo,string origen,string destino,year_month_day fecha,hh_mm_ss<minutes> hora,int capacidad){
    registroVuelos[TOTAL_VUELOS] = new Vuelo(codigoVuelo,origen,destino,fecha,hora,capacidad);
    TOTAL_VUELOS++;
}

void cargarVuelos(){
    FILE* archivo = fopen("../csv_s/vuelos.csv", "r");
	if (archivo == NULL) {
	    cout << "Error al abrir el archivo de lista" << endl;
	    return;
	}
	char linea[500];
	while (fgets(linea, sizeof(linea), archivo) != NULL) {
        char codigoVuelo[10];
        char origen[15];
        char destino[15];
        year_month_day fecha;
        hh_mm_ss<minutes> hora;
        int capacidad;
        
        linea[strcspn(linea, "\r\n")] = '\0'; //Elimina el salto de línea de cada linea

        // Separar campos
        char* token = strtok(linea, ",");

        if (token == NULL) continue;
        strncpy(codigoVuelo, token, sizeof(codigoVuelo) - 1);
        codigoVuelo[sizeof(codigoVuelo) - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(origen, token, sizeof(origen) - 1);
        origen[sizeof(origen) - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(destino, token, sizeof(destino) - 1);
        destino[sizeof(destino) - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        int anio, mes, dia;
        if (sscanf(token, "%d-%d-%d", &anio, &mes, &dia) != 3)
            continue;
        fecha = std::chrono::year{anio} /
                std::chrono::month{static_cast<unsigned>(mes)} /
                std::chrono::day{static_cast<unsigned>(dia)};
        if (!fecha.ok()) continue;

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        int horas, minutos;
        if (sscanf(token, "%d:%d", &horas, &minutos) != 2)
            continue;
        if (horas < 0 || horas > 23 || minutos < 0 || minutos > 59)
            continue;
        hora = std::chrono::hh_mm_ss<std::chrono::minutes>{
            std::chrono::hours{horas} + std::chrono::minutes{minutos}
        };

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        capacidad = atoi(token);

        insertarVuelos(codigoVuelo,origen,destino,fecha,hora,capacidad);
	}
	fclose(archivo);
}
