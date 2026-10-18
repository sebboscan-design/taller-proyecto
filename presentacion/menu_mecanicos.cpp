#include "menu_mecanicos.h"
#include "entrada.h"
#include "../logica/mecanicos.h"
#include <iostream>
using namespace std;

// Muestra el submenu de mecanicos. Los mecanicos no se actualizan ni se eliminan
void menuMecanicos(SistemaTaller* s) {
    // TODO: opciones registrar, buscar, listar y 0 volver
}

// Pide nombre, cedula, especialidad y fecha de ingreso, pide confirmacion
// y llama a registrarMecanico. Segun el Resultado, muestra el mensaje:
// ERROR_DUPLICADO -> cedula repetida, ERROR_DATO_INVALIDO -> dato fuera de lo permitido
void menuRegistrarMecanico(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Pide el ID y muestra el mecanico con buscarMecanicoPorID
void menuBuscarMecanico(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Llama a listarMecanicos, muestra la lista y hace delete[] del arreglo
void menuListarMecanicos(SistemaTaller* s) {
    // TODO: escribir la funcion
}

// Muestra un mecanico con su numero de ordenes ABIERTAS
// (contarOrdenesAbiertasMecanico)
void mostrarMecanico(SistemaTaller* s, Mecanico* mecanico) {
    // TODO: escribir la funcion
}

// Muestra la lista de mecanicos; cada uno con sus ordenes ABIERTAS
void mostrarListaMecanicos(SistemaTaller* s, Mecanico** mecanicos, int cantidad) {
    // TODO: escribir la funcion
}
