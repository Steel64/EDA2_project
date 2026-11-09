all: build

build:
	g++ -c ./base_datos/reserva.cpp
	g++ -c ./base_datos/reservas.cpp
	g++ -c ./base_datos/vuelo.cpp
	g++ -c ./base_datos/vuelos.cpp
	g++ -c ./librerias/registro_hash.cpp
	g++ -c ./librerias/registro_heap.cpp
	g++ -c main.cpp
	g++ reserva.o reservas.o vuelo.o vuelos.o registro_hash.o registro_heap.o main.o -o main.exe
	del /Q *.o
	./main.exe

clean:
	del /Q *.exe