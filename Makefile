all: build

build:
	g++ -c ./base_datos/reserva.cpp
	g++ -c ./base_datos/reservas.cpp
	g++ -c ./base_datos/vuelo.cpp
	g++ -c ./base_datos/vuelos.cpp
	g++ -c ./librerias/registro_hash.cpp
	g++ -c main.cpp
	g++ reserva.o reservas.o vuelo.o vuelos.o registro_hash.o main.o -o main.exe
	./main.exe

clean:
	rm -rf *.o
	rm -rf *.exe