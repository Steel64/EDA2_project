#include "../librerias/gestion_reservas.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

int totalComprobaciones = 0;
int fallidas = 0;

void comprobar(bool condicion, const string& descripcion) {
    totalComprobaciones++;
    if (condicion) cout << "OK: " << descripcion << '\n';
    else { fallidas++; cout << "FALLO: " << descripcion << '\n'; }
}

int main() {
    // BLOQUE 1 — Carga masiva
    cargarVuelos();
    cargarReservas("csv_s/reservas_masivo.csv");
    cargarIndices();
    comprobar(TOTAL_RESERVAS == 10000, "Carga: 10000 reservas");
    comprobar(t_hash.cantidad() == 10000, "Hash: 10000 insertadas");
    comprobar(t_avl.cantidad() == 10000, "AVL: 10000 insertadas");
    cout << "Total en espera: " << TOTAL_ESPERAS << '\n';
    for (int i = 0; i < TOTAL_VUELOS; i++) {
        NodoHeap* h = t_heap.buscarHeap(registroVuelos[i]->getCodigoVuelo());
        if (h && h->tam > 0) cout << "Heap " << h->codVuelo << ": " << h->tam << '\n';
    }

    // BLOQUE 2 — Busqueda por ID
    comprobar(t_hash.buscar("RES-000001") != nullptr, "Buscar primera");
    comprobar(t_hash.buscar("RES-504951") != nullptr, "Buscar intermedia");
    comprobar(t_hash.buscar("RES-509950") != nullptr, "Buscar ultima");
    comprobar(t_hash.buscar("RES-999999") == nullptr, "Buscar inexistente");

    // BLOQUE 3 — Ordenamiento cronologico
    Reserva* lista20[20];
    int cant20;
    t_avl.primeros(lista20, 20, cant20);
    comprobar(cant20 == 20, "Primeros 20 obtenidos");
    for (int i = 0; i < cant20; i++) cout << lista20[i]->getIdReserva() << '\n';

    // BLOQUE 4 — Consulta por rango
    Reserva* resultado[20];
    int cant, total;

    t_avl.rango(year{2027}/1/1, year{2027}/1/7, resultado, 20, cant, total);
    cout << "Semana: " << total << " encontradas\n";
    comprobar(total > 0, "Rango semana tiene resultados");

    t_avl.rango(year{2027}/1/1, year{2027}/1/31, resultado, 20, cant, total);
    cout << "Mes: " << total << " encontradas\n";
    comprobar(total > 0, "Rango mes tiene resultados");

    t_avl.rango(year{2028}/1/1, year{2028}/1/31, resultado, 20, cant, total);
    cout << "Sin resultados: " << total << " encontradas\n";
    comprobar(total == 0, "Rango sin resultados");

    // BLOQUE 5 — Insercion individual
    Vuelo* vueloVU050 = buscarVuelo("VU050");
    Reserva* verificarArbol[5];
    int cantArbolAntes, totalArbolAntes;
    t_avl.rango(vueloVU050->getFecha(), vueloVU050->getFecha(), verificarArbol, 5, cantArbolAntes, totalArbolAntes);

    string error5;
    Reserva* nueva1 = registrarReserva("RES-900001", "VU050", "99999999", "Prueba Uno",
                                        "Economica", "Regular", 150.0f, error5);
    comprobar(nueva1 != nullptr && nueva1->getEstado() == "Confirmada",
              "Insercion en vuelo con cupo -> Confirmada");

    int cantArbolDespues, totalArbolDespues;
    t_avl.rango(vueloVU050->getFecha(), vueloVU050->getFecha(), verificarArbol, 5, cantArbolDespues, totalArbolDespues);
    comprobar(totalArbolDespues == totalArbolAntes + 1, "Nueva reserva aparece en consulta del arbol");

    comprobar(t_hash.buscar("RES-900001") != nullptr, "Nueva reserva aparece en hash");

    string error6;
    Reserva* nueva2 = registrarReserva("RES-900002", "VU001", "99999998", "Prueba Dos",
                                        "Economica", "Regular", 150.0f, error6);
    comprobar(nueva2 != nullptr && nueva2->getEstado() == "EnEspera",
              "Insercion en vuelo lleno -> EnEspera");

    // BLOQUE 6 — Actualizacion y eliminacion
    Reserva* paraActualizar[10];
    int totalParaActualizar = 0;
    for (int i = 0; i < TOTAL_RESERVAS && totalParaActualizar < 10; i++) {
        Reserva* r = registroReservas[i];
        if (r->getCodigoVuelo() == "VU050" && r->getEstado() == "Pendiente") {
            paraActualizar[totalParaActualizar++] = r;
        }
    }
    comprobar(totalParaActualizar == 10, "Se encontraron 10 Pendientes en VU050");
    for (int i = 0; i < totalParaActualizar; i++) {
        bool ok = actualizarEstadoReserva(paraActualizar[i]);
        comprobar(ok && paraActualizar[i]->getEstado() == "Confirmada",
                  "Actualizar " + paraActualizar[i]->getIdReserva());
    }

    Reserva* paraCancelar[10];
    int totalParaCancelar = 0;
    for (int i = 0; i < TOTAL_RESERVAS && totalParaCancelar < 10; i++) {
        Reserva* r = registroReservas[i];
        bool yaElegida = false;
        for (int j = 0; j < totalParaActualizar; j++) if (paraActualizar[j] == r) yaElegida = true;
        if (r->getCodigoVuelo() == "VU050" && r->getEstado() != "Cancelada" && !yaElegida) {
            paraCancelar[totalParaCancelar++] = r;
        }
    }
    comprobar(totalParaCancelar == 10, "Se encontraron 10 para cancelar en VU050");
    for (int i = 0; i < totalParaCancelar; i++) {
        string idGuardado = paraCancelar[i]->getIdReserva();
        Reserva* promovidaTmp = nullptr;
        bool okCancel = cancelarReserva(paraCancelar[i], promovidaTmp);
        comprobar(okCancel, "Cancelar " + idGuardado);
        bool okEliminar = eliminarReservaCancelada(paraCancelar[i]);
        comprobar(okEliminar, "Eliminar fisicamente " + idGuardado);
        comprobar(t_hash.buscar(idGuardado) == nullptr, "Ya no esta en hash: " + idGuardado);
    }

    // BLOQUE 7 — Lista de espera
    Reserva* colaAntes[20];
    int cantColaAntes;
    t_heap.ordenados("VU001", colaAntes, cantColaAntes);
    cout << "\nCola de espera VU001 antes de cancelar (" << cantColaAntes << "):\n";
    for (int i = 0; i < cantColaAntes; i++) {
        cout << "  " << colaAntes[i]->getIdReserva() << " - " << colaAntes[i]->getCategoria() << '\n';
    }
    string idRaizEsperada = colaAntes[0]->getIdReserva();

    Reserva* conAsientoVU001 = nullptr;
    for (int i = 0; i < TOTAL_RESERVAS; i++) {
        Reserva* r = registroReservas[i];
        if (r->getCodigoVuelo() == "VU001" &&
            (r->getEstado() == "Pendiente" || r->getEstado() == "Confirmada" || r->getEstado() == "CheckIn")) {
            conAsientoVU001 = r;
            break;
        }
    }
    comprobar(conAsientoVU001 != nullptr, "Se encontro una reserva con asiento en VU001");

    Reserva* promovida = nullptr;
    bool okCancelAsiento = cancelarReserva(conAsientoVU001, promovida);
    comprobar(okCancelAsiento && promovida != nullptr, "Cancelacion con asiento promueve a alguien");
    comprobar(promovida != nullptr && promovida->getIdReserva() == idRaizEsperada,
              "Se promovio exactamente al que estaba primero en la cola");
    comprobar(promovida != nullptr && promovida->getEstado() == "Confirmada",
              "El promovido queda Confirmada");

    Reserva* colaDespues[20];
    int cantColaDespues;
    t_heap.ordenados("VU001", colaDespues, cantColaDespues);
    comprobar(cantColaDespues == cantColaAntes - 1, "La cola baja en 1 tras la promocion");

    Reserva* noRaiz = colaDespues[cantColaDespues - 1];
    string idNoRaiz = noRaiz->getIdReserva();
    Reserva* promovidaVacia = nullptr;
    bool okCancelEspera = cancelarReserva(noRaiz, promovidaVacia);
    comprobar(okCancelEspera && promovidaVacia == nullptr, "Cancelar EnEspera no genera promocion");

    Reserva* colaFinal[20];
    int cantColaFinal;
    t_heap.ordenados("VU001", colaFinal, cantColaFinal);
    comprobar(cantColaFinal == cantColaDespues - 1, "La cola queda valida tras eliminar un no-raiz");
    bool sigueApareciendo = false;
    for (int i = 0; i < cantColaFinal; i++) {
        if (colaFinal[i]->getIdReserva() == idNoRaiz) sigueApareciendo = true;
    }
    comprobar(!sigueApareciendo, "La reserva eliminada ya no esta en la cola");

    // BLOQUE 8 — Casos extremos
    int encontradosColision = 0;
    for (int i = 0; i < 50; i++) {
        int numero = 1 + i * 10000;
        ostringstream idStream;
        idStream << "RES-" << setfill('0') << setw(6) << numero;
        if (t_hash.buscar(idStream.str()) != nullptr) encontradosColision++;
    }
    comprobar(encontradosColision == 50, "Las 50 reservas con colision se encuentran todas");

    TablaAVL avlPrueba;
    Reserva* reservasPrueba[1000];
    for (int i = 0; i < 1000; i++) {
        ostringstream idStream;
        idStream << "TMP-" << setfill('0') << setw(6) << (i + 1);
        year_month_day fecha = year_month_day{sys_days{year{2030}/1/1} + days{i}};
        hh_mm_ss<minutes> hora{minutes{0}};
        reservasPrueba[i] = new Reserva(idStream.str(), "00000000", "Sintetico", "SIM",
                                         "Origen", "Destino", fecha, hora,
                                         "Economica", "Regular", 100.0f, "Confirmada");
        avlPrueba.insertar(reservasPrueba[i]);
    }
    comprobar(avlPrueba.cantidad() == 1000, "Se insertaron las 1000 reservas sinteticas");
    cout << "Altura del AVL con 1000 fechas crecientes: " << avlPrueba.altura() << '\n';
    comprobar(avlPrueba.altura() < 50, "La altura es logaritmica, no cercana a 1000");

    for (int i = 0; i < 1000; i++) delete reservasPrueba[i];

    cout << "\nTotal comprobaciones: " << totalComprobaciones << ", fallidas: " << fallidas << '\n';
    return 0;
}