#include <iostream>
#include "logica/sistema.h"
#include "presentacion/entrada.h"
#include "presentacion/menu_vehiculos.h"
#include "presentacion/menu_mecanicos.h"
#include "presentacion/menu_repuestos.h"
#include "presentacion/menu_ordenes.h"
using namespace std;

// Muestra el menu principal (formato de la seccion 5 del PDF) y llama al
// submenu elegido. Antes de salir con 0 se pide confirmacion, porque los
// datos en memoria se pierden al cerrar
void menuPrincipal(SistemaTaller* s) {
    // TODO: escribir la funcion
}

int main() {
    SistemaTaller sistema;
    Taller datos = {"Autoservicio El Lago", "J-12345678-9", "Maracaibo", "0261-5551234"};

    inicializarSistema(&sistema, datos);
    menuPrincipal(&sistema);
    liberarSistema(&sistema);
    return 0;
}
