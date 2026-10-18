#ifndef REPUESTOS_H
#define REPUESTOS_H

#include "../estructuras.h"
#include "resultados.h"

// Registra un repuesto nuevo. Codigo unico, precio > 0, stock y stock minimo >= 0
Resultado registrarRepuesto(SistemaTaller* s, Repuesto datos);

// Busca un repuesto. Devuelve su direccion o nullptr si no existe
Repuesto* buscarRepuestoPorID(SistemaTaller* s, int id);
Repuesto* buscarRepuestoPorCodigo(SistemaTaller* s, const char* codigo);

// Devuelve los repuestos cuyo codigo o nombre contiene el texto buscado
// Quien llama debe liberar el arreglo con delete[]
Repuesto** buscarRepuestos(SistemaTaller* s, const char* texto, int* cantidad);

// Devuelve todos los repuestos. Quien llama debe liberar el arreglo con delete[]
Repuesto** listarRepuestos(SistemaTaller* s, int* cantidad);

// Suma unidades al stock cuando llega mercancia. Cantidad > 0
Resultado registrarEntradaMercancia(SistemaTaller* s, int idRepuesto, int cantidad);

// Cambia el precio vigente. No afecta a las ordenes ya registradas
Resultado cambiarPrecioRepuesto(SistemaTaller* s, int idRepuesto, float nuevoPrecio);

// Elimina un repuesto, solo si no aparece en ninguna orden
Resultado eliminarRepuesto(SistemaTaller* s, int idRepuesto);

// true si el stock llego al minimo o menos (para marcar BAJO STOCK)
bool tieneBajoStock(Repuesto* repuesto);

// true si el repuesto aparece en el detalle de alguna orden, en cualquier estado
bool repuestoUsadoEnOrdenes(SistemaTaller* s, int idRepuesto);

#endif
