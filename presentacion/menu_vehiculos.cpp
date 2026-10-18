#include "menu_vehiculos.h"
#include "entrada.h"
#include "../logica/vehiculos.h"
#include <iostream>
using namespace std;

// Muestra el submenu de vehiculos y llama a la opcion elegida
// Se repite hasta que el usuario elija 0 (volver)
void menuVehiculos(SistemaTaller* s) {
    // TODO: opciones registrar, buscar, actualizar, eliminar, listar y 0 volver
}

// Pide los datos del vehiculo, pide confirmacion y llama a agregarVehiculo
// Si en cualquier dato escriben CANCELAR, se sale sin guardar (caso de uso 8)
void menuRegistrarVehiculo(SistemaTaller* s) {
    // TODO: escribir la funcion
    // si agregarVehiculo devuelve nullptr, mostrar por que fallo, por ejemplo:
    // ERROR: La placa 'AB123CD' ya esta registrada
}

// Busca por placa exacta o por parte del nombre del propietario
// Con buscarVehiculosPorPropietario, al final hacer delete[] del arreglo
void menuBuscarVehiculo(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide el ID, muestra el vehiculo, pide los datos nuevos y confirmacion,
// y llama a actualizarVehiculo
void menuActualizarVehiculo(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide el ID, pide confirmacion y llama a eliminarVehiculo
// Si tiene ordenes, el mensaje debe decir cuantas (contarOrdenesDeVehiculo)
void menuEliminarVehiculo(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Llama a listarVehiculos, muestra la lista y hace delete[] del arreglo
void menuListarVehiculos(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Muestra los datos de un solo vehiculo
void mostrarVehiculo(Vehiculo* vehiculo) {
    // TODO: escribir la funcion
}

// Muestra una lista de vehiculos en forma de tabla
void mostrarListaVehiculos(Vehiculo** vehiculos, int cantidad) {
    // TODO: escribir la funcion
}
