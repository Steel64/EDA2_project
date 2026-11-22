#include "../librerias/gestion_reservas.h"
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
using namespace std::chrono;

double microsegundosDesde(steady_clock::time_point inicio) {
    return duration<double, micro>(steady_clock::now() - inicio).count();
}

Reserva* buscarSecuencial(const string& id) {
    for (int i = 0; i < TOTAL_RESERVAS; i++) {
        if (registroReservas[i]->getIdReserva() == id) return registroReservas[i];
    }
    return nullptr;
}

int main() {
    ofstream resultados("pruebas/resultados.csv");
    ofstream metricas("pruebas/metricas.csv");
    resultados << "operacion,repeticiones,total_us,promedio_us\n";

    // Carga masiva
    auto inicioCarga = steady_clock::now();
    cargarVuelos();
    cargarReservas("csv_s/reservas_masivo.csv");
    cargarIndices();
    double tiempoCargaUs = microsegundosDesde(inicioCarga);
    resultados << "carga_masiva,1," << tiempoCargaUs << "," << tiempoCargaUs << "\n";
    cout << "Carga masiva: " << tiempoCargaUs / 1000.0 << " ms\n";

    // Busqueda por ID (hash) vs secuencial
    const int REP_BUSQUEDA = 1000;
    string idBusqueda = "RES-504951";

    auto inicioHash = steady_clock::now();
    for (int i = 0; i < REP_BUSQUEDA; i++) t_hash.buscar(idBusqueda);
    double tiempoHash = microsegundosDesde(inicioHash);
    resultados << "busqueda_hash," << REP_BUSQUEDA << "," << tiempoHash << "," << tiempoHash / REP_BUSQUEDA << "\n";
    cout << "Busqueda hash promedio: " << tiempoHash / REP_BUSQUEDA << " us\n";

    auto inicioSecuencial = steady_clock::now();
    for (int i = 0; i < REP_BUSQUEDA; i++) buscarSecuencial(idBusqueda);
    double tiempoSecuencial = microsegundosDesde(inicioSecuencial);
    resultados << "busqueda_secuencial," << REP_BUSQUEDA << "," << tiempoSecuencial << "," << tiempoSecuencial / REP_BUSQUEDA << "\n";
    cout << "Busqueda secuencial promedio: " << tiempoSecuencial / REP_BUSQUEDA << " us\n";

    // Insercion individual
    const int REP_INSERCION = 100;
    double tiempoInsercionTotal = 0;
    for (int i = 0; i < REP_INSERCION; i++) {
        string error;
        auto inicio = steady_clock::now();
        Reserva* nueva = registrarReserva("RES-950000", "VU050", "88888888", "Medicion Temporal",
                                            "Economica", "Regular", 100.0f, error);
        tiempoInsercionTotal += microsegundosDesde(inicio);
        if (nueva) {
            Reserva* promovidaTmp = nullptr;
            cancelarReserva(nueva, promovidaTmp);
            eliminarReservaCancelada(nueva);
        }
    }
    resultados << "insercion_individual," << REP_INSERCION << "," << tiempoInsercionTotal << "," << tiempoInsercionTotal / REP_INSERCION << "\n";
    cout << "Insercion individual promedio: " << tiempoInsercionTotal / REP_INSERCION << " us\n";

    // Consulta ordenada (primeras 50)
    const int REP_CONSULTA = 100;
    Reserva* lista50[50];
    int cant50;
    auto inicioOrdenada = steady_clock::now();
    for (int i = 0; i < REP_CONSULTA; i++) t_avl.primeros(lista50, 50, cant50);
    double tiempoOrdenada = microsegundosDesde(inicioOrdenada);
    resultados << "consulta_ordenada," << REP_CONSULTA << "," << tiempoOrdenada << "," << tiempoOrdenada / REP_CONSULTA << "\n";
    cout << "Consulta ordenada promedio: " << tiempoOrdenada / REP_CONSULTA << " us\n";

    // Consulta por rango (un mes)
    Reserva* listaRango[20];
    int cantRango, totalRango;
    auto inicioRango = steady_clock::now();
    for (int i = 0; i < REP_CONSULTA; i++) {
        t_avl.rango(year{2027}/1/1, year{2027}/1/31, listaRango, 20, cantRango, totalRango);
    }
    double tiempoRango = microsegundosDesde(inicioRango);
    resultados << "consulta_rango_mes," << REP_CONSULTA << "," << tiempoRango << "," << tiempoRango / REP_CONSULTA << "\n";
    cout << "Consulta por rango promedio: " << tiempoRango / REP_CONSULTA << " us\n";
     metricas << "metrica,valor\n";
    metricas << "reservas," << TOTAL_RESERVAS << "\n";
    metricas << "buckets," << TAMANIO << "\n";
    metricas << "factor_carga," << t_hash.factorCarga() << "\n";
    metricas << "cadena_mas_larga," << t_hash.cadenaMasLarga() << "\n";
    metricas << "altura_avl_10000," << t_avl.altura() << "\n";
    // Actualizacion de estado: 10 reservas distintas de VU050
    Reserva* diezActualizar[10];
    int totalDiezActualizar = 0;
    for (int i = 0; i < TOTAL_RESERVAS && totalDiezActualizar < 10; i++) {
        if (registroReservas[i]->getCodigoVuelo() == "VU050" && registroReservas[i]->getEstado() == "Pendiente") {
            diezActualizar[totalDiezActualizar++] = registroReservas[i];
        }
    }
    double tiempoActualizar = 0;
    for (int i = 0; i < totalDiezActualizar; i++) {
        auto inicio = steady_clock::now();
        actualizarEstadoReserva(diezActualizar[i]);
        tiempoActualizar += microsegundosDesde(inicio);
    }
    resultados << "actualizar_estado," << totalDiezActualizar << "," << tiempoActualizar << "," << tiempoActualizar / totalDiezActualizar << "\n";
    cout << "Actualizar estado promedio: " << tiempoActualizar / totalDiezActualizar << " us\n";

    // Cancelacion con promocion: 10 reservas con asiento en VU002 (vuelo lleno distinto de VU001)
    Reserva* diezCancelar[10];
    int totalDiezCancelar = 0;
    for (int i = 0; i < TOTAL_RESERVAS && totalDiezCancelar < 10; i++) {
        Reserva* r = registroReservas[i];
        if (r->getCodigoVuelo() == "VU002" &&
            (r->getEstado() == "Pendiente" || r->getEstado() == "Confirmada" || r->getEstado() == "CheckIn")) {
            diezCancelar[totalDiezCancelar++] = r;
        }
    }
    double tiempoCancelar = 0;
    for (int i = 0; i < totalDiezCancelar; i++) {
        Reserva* promovidaTmp = nullptr;
        auto inicio = steady_clock::now();
        cancelarReserva(diezCancelar[i], promovidaTmp);
        tiempoCancelar += microsegundosDesde(inicio);
    }
    resultados << "cancelar_con_promocion," << totalDiezCancelar << "," << tiempoCancelar << "," << tiempoCancelar / totalDiezCancelar << "\n";
    cout << "Cancelar con promocion promedio: " << tiempoCancelar / totalDiezCancelar << " us\n";

    // Eliminacion fisica: las mismas 10 que acabamos de cancelar
    double tiempoEliminar = 0;
    for (int i = 0; i < totalDiezCancelar; i++) {
        auto inicio = steady_clock::now();
        eliminarReservaCancelada(diezCancelar[i]);
        tiempoEliminar += microsegundosDesde(inicio);
    }
    resultados << "eliminar_fisica," << totalDiezCancelar << "," << tiempoEliminar << "," << tiempoEliminar / totalDiezCancelar << "\n";
    cout << "Eliminar fisica promedio: " << tiempoEliminar / totalDiezCancelar << " us\n";


    resultados.close();
    metricas.close();
    cout << "\nOK: resultados.csv y metricas.csv guardados\n";
    return 0;
}