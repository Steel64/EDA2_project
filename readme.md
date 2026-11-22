### Instalacion de MinGW-w64 for Windows

Link de instalación: https://winlibs.com/

- En GCC 16.2.0 (with POSIX threads) + LLVM/Clang/LLD/LLDB 23.1.3 + MinGW-w64 14.0.0 (UCRT)
- Descargar el archivo de "Win64 (without LLVM/Clang/LLD/LLDB)"
- Descomprime el archivo
- Coloca la carpeta en C:\mingw64
- Busca la carpeta bin que contiene g++.exe.
- Agrega esa carpeta de BIN como variable de entorno en el PATH.
- Reinicia VS Code.
- Ejecuta:
```
g++ --version
mingw32-make --version
```
- Debe obtener la versiones de ambos.

### 1. Ejecutar el sistema principal

Se requiere un compilador con soporte de C++20. Desde la carpeta del proyecto:

```
mingw32-make
```

El programa siempre carga `csv_s/vuelos.csv`. El argumento selecciona el archivo
de reservas: `0` carga `csv_s/reservas_pequeno.csv` y `1` carga
`csv_s/reservas_masivo.csv`.

```
main.exe 0
main.exe 1
```

### 2. Generar los datos de prueba (opcional, ya están generados en csv_s/)

Si se necesita regenerar los 3 archivos CSV desde cero:

```
g++ -std=c++20 -Wall -Wextra generador_datos.cpp -o generador_datos.exe
.\generador_datos.exe
```

Esto reemplaza `csv_s/vuelos.csv`, `csv_s/reservas_pequeno.csv` y `csv_s/reservas_masivo.csv`.

### 3. Correr los escenarios de prueba (sección 4 del enunciado)

```
g++ -std=c++20 -Wall -Wextra pruebas/escenarios.cpp base_datos/*.cpp librerias/*.cpp -o pruebas_escenarios.exe
.\pruebas_escenarios.exe
```

Debe terminar mostrando `Total comprobaciones: 68, fallidas: 0`.

### 4. Medir los tiempos de ejecución (para el informe)

```
g++ -std=c++20 -Wall -Wextra -O2 pruebas/rendimiento.cpp base_datos/*.cpp librerias/*.cpp -o rendimiento.exe
.\rendimiento.exe
```

Esto genera/actualiza `pruebas/resultados.csv` y `pruebas/metricas.csv` con los tiempos y métricas estructurales.

### 5. Ver los resultados ya generados

Estos comandos muestran los resultados ya guardados, sin necesidad de correr rendimiento.exe de nuevo:

```
Get-Content pruebas/resultados.csv
Get-Content pruebas/metricas.csv
```