#include "menu_repuestos.h"
#include "entrada.h"
#include "../logica/repuestos.h"
#include <iostream>
using namespace std;

// Muestra el submenu de inventario
void menuRepuestos(SistemaTaller* s) {
    // TODO: opciones registrar, buscar, entrada de mercancia, cambiar precio,
    // eliminar, listar y 0 volver
}

// Pide codigo, nombre, marca, precio, stock y stock minimo, pide confirmacion
// y llama a registrarRepuesto. Mostrar el mensaje segun el Resultado
void menuRegistrarRepuesto(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Busca por codigo o por parte del nombre con buscarRepuestos
// Al final, delete[] del arreglo
void menuBuscarRepuesto(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide el ID y la cantidad que llego, y llama a registrarEntradaMercancia
void menuEntradaMercancia(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide el ID y el precio nuevo, y llama a cambiarPrecioRepuesto
void menuCambiarPrecio(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide el ID, pide confirmacion y llama a eliminarRepuesto
// ERROR_EN_USO -> el repuesto aparece en alguna orden
void menuEliminarRepuesto(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Llama a listarRepuestos, muestra la lista y hace delete[] del arreglo
void menuListarRepuestos(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Muestra los datos de un repuesto
void mostrarRepuesto(Repuesto* repuesto) {
    // TODO: escribir la funcion
}

// Muestra la lista de repuestos. Los que cumplan tieneBajoStock
// se marcan con BAJO STOCK (caso de uso 2)
void mostrarListaRepuestos(Repuesto** repuestos, int cantidad) {
    // TODO: escribir la funcion
}
