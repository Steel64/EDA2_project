#include "reservas.h"
#include <cctype>
#include <cstring>
#include <cstdio>
#include <iostream>

int TOTAL_RESERVAS = 0;
Reserva * registroReservas[MAX_RESERVAS];

void insertarReserva(string idReserva,string dni,string pasajero,string codigoVuelo,string origen,string destino,year_month_day fecha,hh_mm_ss<minutes> hora,string clase,string categoria,float precio,string estado){
    registroReservas[TOTAL_RESERVAS] = new Reserva(idReserva,dni,pasajero,codigoVuelo,origen,destino,fecha,hora,clase,categoria,precio,estado);
    TOTAL_RESERVAS++;
}

void cargarReservas(){
	FILE* archivo = fopen("../csv_s/reservas_pequeno.csv", "r");
    //FILE* archivo = fopen("../csv_s/reservas_masivo.csv", "r");
	if (archivo == NULL) {
	    cout << "Error al abrir el archivo de lista" << endl;
	    return;
	}
	char linea[500];
	while (fgets(linea, sizeof(linea), archivo) != NULL) {
        char idReserva[12];
        char dni[10];
        char pasajero[30];
        char codigoVuelo[15];
        char origen[15];
        char destino[15];
        year_month_day fecha;
        hh_mm_ss<minutes> hora;
        char clase[11];
        char categoria[11];
        float precio;
        char estado[12];
        
        linea[strcspn(linea, "\r\n")] = '\0'; //Elimina el salto de línea de cada linea

        // Separar campos
        char* token = strtok(linea, ",");

        if (token == NULL) continue;
        strncpy(idReserva, token, sizeof(idReserva) - 1);
        idReserva[sizeof(idReserva) - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(dni, token, sizeof(dni) - 1);
        pasajero[sizeof(dni) - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(pasajero, token, sizeof(pasajero) - 1);
        pasajero[sizeof(pasajero) - 1] = '\0';

        token = strtok(NULL, ",");
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
        strncpy(clase, token, sizeof(clase) - 1);
        clase[sizeof(clase) - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(categoria, token, sizeof(categoria) - 1);
        categoria[sizeof(categoria) - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        precio = atof(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(estado, token, sizeof(estado) - 1);
        estado[sizeof(estado) - 1] = '\0';

        insertarReserva(idReserva,dni,pasajero,codigoVuelo,origen,destino,fecha,hora,clase,categoria,precio,estado);
	}
	fclose(archivo);
}
