#ifndef MECANICOS_H
#define MECANICOS_H

#include "../estructuras.h"
#include "resultados.h"

// Registra un mecanico nuevo. Cedula unica y especialidad permitida
Resultado registrarMecanico(SistemaTaller* s, Mecanico datos);

// Busca un mecanico. Devuelve su direccion o nullptr si no existe
Mecanico* buscarMecanicoPorID(SistemaTaller* s, int id);
Mecanico* buscarMecanicoPorCedula(SistemaTaller* s, const char* cedula);

// Devuelve un arreglo nuevo con todos los mecanicos
// Quien llama debe liberarlo con delete[]
Mecanico** listarMecanicos(SistemaTaller* s, int* cantidad);

// Cuantas ordenes ABIERTAS tiene un mecanico (el maximo permitido es 3)
int contarOrdenesAbiertasMecanico(SistemaTaller* s, int idMecanico);

#endif
