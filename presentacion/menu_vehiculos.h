#ifndef MENU_VEHICULOS_H
#define MENU_VEHICULOS_H

#include "../estructuras.h"

// Submenu de la opcion 1 del menu principal
void menuVehiculos(SistemaTaller* s);

// Firmas del enunciado (seccion 3.2)
void menuRegistrarVehiculo(SistemaTaller* s);
void menuBuscarVehiculo(SistemaTaller* s);
void menuActualizarVehiculo(SistemaTaller* s);
void menuEliminarVehiculo(SistemaTaller* s);
void menuListarVehiculos(SistemaTaller* s);
void mostrarVehiculo(Vehiculo* vehiculo);
void mostrarListaVehiculos(Vehiculo** vehiculos, int cantidad);

#endif
