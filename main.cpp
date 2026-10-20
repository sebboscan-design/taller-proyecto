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
    int opcion = -1;
    do {
        cout << "\n+-------------------------------------------+\n";
        cout << "  SISTEMA DE GESTION DE TALLER\n";
        cout << "  Taller: " << s->taller.nombre << "\n";
        cout << "+-------------------------------------------+\n";
        cout << "1. Gestion de Vehiculos\n";
        cout << "2. Gestion de Mecanicos\n";
        cout << "3. Inventario de Repuestos\n";
        cout << "4. Ordenes de Servicio\n";
        cout << "0. Salir\n";
        opcion = leerOpcion("Seleccione una opcion", 0, 4);

        switch (opcion) {
            case 1: menuVehiculos(s); break;
            case 2: menuMecanicos(s); break;
            case 3: menuRepuestos(s); break;
            case 4: menuOrdenes(s); break;
            case 0:
                // si dice que no, se pone otra opcion para que el ciclo siga
                if (!confirmar("Los datos se perderan al salir. Desea salir? (S/N): ")) {
                    opcion = 5;
                }
                break;
        }
    } while (opcion != 0 && opcion != -1); // -1: se acabo la entrada

    cout << "Hasta luego.\n";
}

int main() {
    SistemaTaller sistema;
    Taller datos = {"Autoservicio El Lago", "J-12345678-9", "Maracaibo", "0261-5551234"};

    inicializarSistema(&sistema, datos);
    menuPrincipal(&sistema);
    liberarSistema(&sistema);
    return 0;
}
