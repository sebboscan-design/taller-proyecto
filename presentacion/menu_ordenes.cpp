#include "menu_ordenes.h"
#include "entrada.h"
#include "../logica/ordenes.h"
#include "../logica/vehiculos.h"
#include "../logica/mecanicos.h"
#include "../logica/repuestos.h"
#include <iostream>
using namespace std;

// Muestra el submenu de ordenes de servicio
void menuOrdenes(SistemaTaller* s) {
    // TODO: opciones 1 a 7 de la seccion 5 del PDF y 0 volver
}

// Pide placa (o ID) del vehiculo, ID del mecanico, kilometraje y falla,
// pide confirmacion y llama a abrirOrden. Segun el Resultado, por ejemplo:
// ERROR_VEHICULO_CON_ORDEN_ABIERTA -> El vehiculo 'AB123CD' ya tiene la orden #4 ABIERTA
// ERROR_MECANICO_SIN_CUPO          -> El mecanico 'Jose Perez' ya tiene 3 ordenes abiertas
// ERROR_KILOMETRAJE_MENOR          -> el km es menor al ultimo registrado
void menuAbrirOrden(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide la orden, el repuesto y la cantidad, y llama a agregarRepuestoAOrden
// ERROR_STOCK_INSUFICIENTE -> Stock insuficiente de 'X'. Disponible: 2, solicitado: 5
// ERROR_ESTADO_INVALIDO    -> La orden #3 esta CERRADA y no puede modificarse
void menuAgregarRepuestoOrden(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide la orden y el repuesto, y llama a quitarRepuestoDeOrden
void menuQuitarRepuestoOrden(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide la orden y la mano de obra, pide confirmacion y llama a cerrarOrden
// ERROR_ORDEN_VACIA -> no tiene repuestos y la mano de obra es 0
void menuCerrarOrden(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide la orden, pide confirmacion y llama a anularOrden
void menuAnularOrden(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide el ID y muestra el detalle completo con mostrarDetalleOrden
void menuVerDetalleOrden(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Lista todas, por estado o por vehiculo (historial)
// Al final, delete[] del arreglo que devuelva la funcion de logica
void menuListarOrdenes(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Muestra la orden con el formato de la seccion 9.1 del PDF:
// estado, fechas, vehiculo, dueno, mecanico, falla, lineas de repuestos,
// subtotal, mano de obra y total (calcularSubtotalRepuestos y calcularTotalOrden)
void mostrarDetalleOrden(SistemaTaller* s, OrdenServicio* orden) {
    // TODO: escribir la funcion
}

// Muestra una lista corta de ordenes: numero, placa, estado y total
void mostrarListaOrdenes(SistemaTaller* s, OrdenServicio** ordenes, int cantidad) {
    // TODO: escribir la funcion
}
