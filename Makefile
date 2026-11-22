CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
SOURCES = main.cpp base_datos/reserva.cpp base_datos/reservas.cpp base_datos/vuelo.cpp base_datos/vuelos.cpp librerias/registro_hash.cpp librerias/registro_heap.cpp librerias/registro_AVL.cpp librerias/gestion_reservas.cpp
LIB_SOURCES = $(filter-out main.cpp,$(SOURCES))
HEADERS = $(wildcard base_datos/*.h librerias/*.h)

all: build

build: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o main.exe

clean:
	del /Q *.exe
