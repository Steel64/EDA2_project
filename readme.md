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

### Ejecutar proyecto

Antes de ejecutar, selecciona el tamanio del hash, dependiendo si es reservas_pequeño.csv o reservas.masivo.csv
```
En la librerias/registro_hash.h (linea 7 a 10)
// usar para csv de reservas_pequeñas.csv
//const int TAMANIO = 10;
// usar para csv de reservas_masivo.csv
const int TAMANIO = 2000;
```

Se requiere un compilador con soporte de C++20. Desde la carpeta del proyecto:

```
mingw32-make
./main.exe
```

El programa siempre carga `csv_s/vuelos.csv`. El argumento selecciona el archivo
de reservas: `0` carga `csv_s/reservas_pequeno.csv` y `1` carga
`csv_s/reservas_masivo.csv`. Ejecuta desde la carpeta del proyecto:

```
main.exe 0
main.exe 1
```