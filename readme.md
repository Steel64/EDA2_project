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

#### En windows
- Usar el comando:
```
    mingw32-make
```
