#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

const int CANTIDAD_VUELOS = 50;
const int RESERVAS_PEQUENAS = 30;
const int RESERVAS_MASIVAS = 10000;
const int VUELOS_LLENOS = 3;
const int ASIENTOS_VUELO_LLENO = 150;
const int ESPERAS_POR_VUELO = 15;

struct DatosVuelo
{
    string codigo;
    string origen;
    string destino;
    string fecha;
    string hora;
    int capacidad;
};

void generarVuelos(ofstream& archivo, DatosVuelo datos[])
{
    const int CANTIDAD_CIUDADES = 10;

    const string ciudades[CANTIDAD_CIUDADES] = {
        "Lima", "Cusco", "Arequipa", "Piura", "Iquitos",
        "Tacna", "Trujillo", "Chiclayo", "Tarapoto", "Pucallpa"
    };

    const string horas[4] = {
        "08:00", "10:30", "14:15", "18:45"
    };

    const string meses[3] = {
        "01", "02", "03"
    };

    for (int i = 0; i < CANTIDAD_VUELOS; i++)
    {
        DatosVuelo vuelo;

        ostringstream codigoStream;
        codigoStream << "VU"
                     << setfill('0')
                     << setw(3)
                     << (i + 1);
        vuelo.codigo = codigoStream.str();

        vuelo.origen = ciudades[i % CANTIDAD_CIUDADES];
        vuelo.destino =
            ciudades[(i * 3 + 2) % CANTIDAD_CIUDADES];

        if (vuelo.destino == vuelo.origen)
        {
            vuelo.destino =
                ciudades[(i * 3 + 3) % CANTIDAD_CIUDADES];
        }

        int mesIndice;
        int dia;

        if (i == 1)
        {
            mesIndice = 0;
            dia = 1;
        }
        else
        {
            mesIndice = i / 20;
            dia = (i % 20) + 1;
        }

        ostringstream fechaStream;
        fechaStream << "2027-"
                    << meses[mesIndice]
                    << "-"
                    << setfill('0')
                    << setw(2)
                    << dia;
        vuelo.fecha = fechaStream.str();

        if (i == 1)
        {
            vuelo.hora = horas[0];
        }
        else
        {
            vuelo.hora = horas[i % 4];
        }

        if (i < VUELOS_LLENOS)
        {
            vuelo.capacidad = ASIENTOS_VUELO_LLENO;
        }
        else
        {
            vuelo.capacidad = 150 + (i % 10) * 15;
        }

        archivo << vuelo.codigo << ","
                << vuelo.origen << ","
                << vuelo.destino << ","
                << vuelo.fecha << ","
                << vuelo.hora << ","
                << vuelo.capacidad << '\n';

        datos[i] = vuelo;
    }
}

string idParaIndice(int i, bool esMasivo)
{
    int numero;

    if (esMasivo && i < 50)
    {
        numero = 1 + i * 10000;
    }
    else if (esMasivo)
    {
        numero = 500001 + (i - 50);
    }
    else
    {
        numero = i + 1;
    }

    ostringstream idStream;
    idStream << "RES-"
             << setfill('0')
             << setw(6)
             << numero;

    return idStream.str();
}
void generarReservas(
    const string& ruta,
    int cantidad,
    bool esMasivo,
    DatosVuelo datos[])
{
    ofstream archivo(ruta);

    if (!archivo)
    {
        cerr << "No se pudo abrir " << ruta << endl;
        return;
    }

    int ocupados[CANTIDAD_VUELOS] = {};

    const string nombres[] = {
        "Sebastian", "Maria", "Juan", "Lucia", "Carlos",
        "Ana", "Pedro", "Camila", "Diego", "Valeria",
        "Luis", "Daniela", "Jose", "Sofia", "Miguel",
        "Andrea", "Fernando", "Claudia", "Ricardo", "Elena"
    };

    const string apellidos[] = {
        "Perez", "Lopez", "Garcia", "Rodriguez", "Martinez",
        "Gonzales", "Flores", "Ramirez", "Torres", "Vargas",
        "Rojas", "Diaz", "Castillo", "Morales", "Romero"
    };

    const string clases[] = {
        "Economica",
        "Premium",
        "Ejecutiva"
    };

    const int cantidadNombres = 20;
    const int cantidadApellidos = 15;

    archivo << fixed << setprecision(2);

    for (int i = 0; i < cantidad; i++)
    {
        int vuelo;
        int posLocal;
        int posEspera;
        string estado;
        string categoria;

        if (esMasivo &&
            i < VUELOS_LLENOS *
                (ASIENTOS_VUELO_LLENO + ESPERAS_POR_VUELO))
        {
            vuelo = i /
                (ASIENTOS_VUELO_LLENO + ESPERAS_POR_VUELO);

            posLocal = i %
                (ASIENTOS_VUELO_LLENO + ESPERAS_POR_VUELO);

            if (posLocal < ASIENTOS_VUELO_LLENO)
            {
                if (i % 3 == 0)
                    estado = "Pendiente";
                else if (i % 3 == 1)
                    estado = "Confirmada";
                else
                    estado = "CheckIn";

                if (i % 3 == 0)
                    categoria = "Regular";
                else if (i % 3 == 1)
                    categoria = "Frecuente";
                else
                    categoria = "Premium";
            }
            else
            {
                estado = "EnEspera";
                posEspera =
                    posLocal - ASIENTOS_VUELO_LLENO;

                if (posEspera < 5)
                    categoria = "Premium";
                else if (posEspera < 10)
                    categoria = "Frecuente";
                else
                    categoria = "Regular";
            }
        }
        else
        {
            int base = esMasivo ? 495 : 0;
            int resto = i - base;

            vuelo = VUELOS_LLENOS +
                resto % (CANTIDAD_VUELOS - VUELOS_LLENOS);

            if (ocupados[vuelo] < datos[vuelo].capacidad)
            {
                if (i % 3 == 0)
                    estado = "Pendiente";
                else if (i % 3 == 1)
                    estado = "Confirmada";
                else
                    estado = "CheckIn";

                if (i % 3 == 0)
                    categoria = "Regular";
                else if (i % 3 == 1)
                    categoria = "Frecuente";
                else
                    categoria = "Premium";

                ocupados[vuelo]++;
            }
            else
            {
                estado = "EnEspera";

                if (i % 3 == 0)
                    categoria = "Premium";
                else if (i % 3 == 1)
                    categoria = "Frecuente";
                else
                    categoria = "Regular";
            }
        }

        string clase = clases[i % 3];

        string nombre =
            nombres[i % cantidadNombres];

        string apellido =
            apellidos[(i / cantidadNombres) %
                      cantidadApellidos];

        string pasajero = nombre + " " + apellido;

        ostringstream dniStream;
        dniStream << setfill('0')
                  << setw(8)
                  << (10000000 + i);

        string dni = dniStream.str();

        double precio =
            80.0 +
            (i % 120) * 1.25 +
            (i % 3) * 180.0;

        string idReserva = idParaIndice(i, esMasivo);

        archivo << idReserva << ","
                << dni << ","
                << pasajero << ","
                << datos[vuelo].codigo << ","
                << datos[vuelo].origen << ","
                << datos[vuelo].destino << ","
                << datos[vuelo].fecha << ","
                << datos[vuelo].hora << ","
                << clase << ","
                << categoria << ","
                << precio << ","
                << estado << '\n';
    }

    archivo.close();
}

int main()
{
    DatosVuelo datosVuelos[CANTIDAD_VUELOS];

    ofstream archivoVuelos("csv_s/vuelos.csv");
    if (!archivoVuelos)
    {
        cerr << "No se pudo abrir csv_s/vuelos.csv" << endl;
        return 1;
    }
    generarVuelos(archivoVuelos, datosVuelos);
    archivoVuelos.close();

    generarReservas("csv_s/reservas_pequeno.csv", RESERVAS_PEQUENAS, false, datosVuelos);
    generarReservas("csv_s/reservas_masivo.csv", RESERVAS_MASIVAS, true, datosVuelos);

    cout << "Generados: " << CANTIDAD_VUELOS << " vuelos, "
         << RESERVAS_PEQUENAS << " reservas pequenas y "
         << RESERVAS_MASIVAS << " reservas masivas." << endl;

    return 0;
}