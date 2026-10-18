#ifndef VEHICULOS_H
#define VEHICULOS_H

#include "../estructuras.h"

// Firmas del enunciado (seccion 3.1), no se cambian

// Registra un carro nuevo. Si algo no cuadra (placa repetida, anio raro,
// km negativo) devuelve nullptr y no guarda nada
Vehiculo* agregarVehiculo(SistemaTaller* s, Vehiculo datos);

// El carro con ese ID, o nullptr si no esta
Vehiculo* buscarVehiculoPorID(SistemaTaller* s, int id);

// El carro con esa placa exacta, o nullptr
Vehiculo* buscarVehiculoPorPlaca(SistemaTaller* s, const char* placa);

// Todos los carros cuyo dueno contiene ese texto ("gonz" -> "Gonzalez")
// Quien llama hace el delete[] del arreglo que devuelve
Vehiculo** buscarVehiculosPorPropietario(SistemaTaller* s, const char* subcadena, int* cantidad);

// Todos los carros. Quien llama hace el delete[]
Vehiculo** listarVehiculos(SistemaTaller* s, int* cantidad);

// Cambia marca, modelo, anio y datos del dueno
// La placa, el ID, el km y la fecha de registro NO se tocan
bool actualizarVehiculo(SistemaTaller* s, int id, Vehiculo datos);

// Lo borra del arreglo, pero solo si nunca tuvo ordenes de servicio
bool eliminarVehiculo(SistemaTaller* s, int id);

#endif
