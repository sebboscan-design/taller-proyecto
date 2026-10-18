#ifndef ORDENES_H
#define ORDENES_H

#include "../estructuras.h"
#include "resultados.h"

// Abre una orden nueva en estado ABIERTA. Si todo sale bien, escribe en
// *idNuevaOrden el ID asignado
Resultado abrirOrden(SistemaTaller* s, int idVehiculo, int idMecanico,
                     int kilometrajeIngreso, const char* descripcionFalla, int* idNuevaOrden);

// Agrega unidades de un repuesto a una orden ABIERTA y las descuenta del stock
// Si el repuesto ya estaba en la orden, se suman a esa misma linea
Resultado agregarRepuestoAOrden(SistemaTaller* s, int idOrden, int idRepuesto, int cantidad);

// Quita la linea completa de un repuesto y devuelve sus unidades al stock
Resultado quitarRepuestoDeOrden(SistemaTaller* s, int idOrden, int idRepuesto);

// Cierra una orden ABIERTA registrando la mano de obra
Resultado cerrarOrden(SistemaTaller* s, int idOrden, float manoObra);

// Anula una orden ABIERTA o CERRADA y devuelve al stock todos sus repuestos
Resultado anularOrden(SistemaTaller* s, int idOrden);

// Totales: no se guardan en la orden, siempre se calculan a partir de sus lineas
float calcularSubtotalRepuestos(OrdenServicio* orden);
float calcularTotalOrden(OrdenServicio* orden);

// Busquedas. Devuelven la direccion encontrada o nullptr
OrdenServicio* buscarOrdenPorID(SistemaTaller* s, int id);
OrdenServicio* buscarOrdenAbiertaDeVehiculo(SistemaTaller* s, int idVehiculo);
DetalleOrden* buscarDetalleEnOrden(OrdenServicio* orden, int idRepuesto);

// Cuantas ordenes tiene un vehiculo, en cualquier estado
int contarOrdenesDeVehiculo(SistemaTaller* s, int idVehiculo);

// Listados. Quien llama debe liberar el arreglo con delete[]
OrdenServicio** listarOrdenes(SistemaTaller* s, int* cantidad);
OrdenServicio** listarOrdenesPorEstado(SistemaTaller* s, const char* estado, int* cantidad);
OrdenServicio** listarOrdenesPorVehiculo(SistemaTaller* s, int idVehiculo, int* cantidad);

#endif
