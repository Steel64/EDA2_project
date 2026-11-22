#pragma once
#include <cstdio>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Los archivos del proyecto no usan campos entre comillas ni comas en los nombres.
inline bool separarCampos(string linea, string campos[], int cantidad) {
    if (!linea.empty() && linea.back()=='\r') linea.pop_back();
    istringstream fila(linea);
    for (int i=0; i<cantidad; ++i) {
        if (!getline(fila,campos[i],',') || campos[i].empty()) return false;
    }
    return fila.eof();
}

inline bool leerSalida(const string& fecha, const string& hora,
                       year_month_day& diaSalida, hh_mm_ss<minutes>& horaSalida) {
    int anio,mes,dia,horas,minutos;
    if (sscanf(fecha.c_str(),"%d-%d-%d",&anio,&mes,&dia)!=3) return false;
    if (sscanf(hora.c_str(),"%d:%d",&horas,&minutos)!=2) return false;
    if (mes<1 || mes>12 || dia<1 || dia>31 || horas<0 || horas>23 || minutos<0 || minutos>59) return false;
    diaSalida=year{anio}/month{static_cast<unsigned>(mes)}/day{static_cast<unsigned>(dia)};
    horaSalida=hh_mm_ss<minutes>{hours{horas}+minutes{minutos}};
    return diaSalida.ok();
}
