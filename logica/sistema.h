#ifndef SISTEMA_H
#define SISTEMA_H

#include "../estructuras.h"

const int CAPACIDAD_INICIAL_ENTIDADES = 4;
const int CAPACIDAD_INICIAL_DETALLES = 2;

// Arranque y cierre del taller
void inicializarSistema(SistemaTaller* s, Taller datosTaller);
void liberarSistema(SistemaTaller* s);

// Se llaman justo antes de insertar: si el arreglo esta lleno, duplica la capacidad
void asegurarCapacidadVehiculos(SistemaTaller* s);
void asegurarCapacidadMecanicos(SistemaTaller* s);
void asegurarCapacidadRepuestos(SistemaTaller* s);
void asegurarCapacidadOrdenes(SistemaTaller* s);
void asegurarCapacidadDetalles(OrdenServicio* orden);

#endif