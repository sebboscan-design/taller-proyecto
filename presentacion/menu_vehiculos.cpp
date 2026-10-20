#include "menu_vehiculos.h"
#include "entrada.h"
#include "../logica/vehiculos.h"
#include "../logica/ordenes.h"
#include "../logica/utilidades.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// Muestra el submenu de vehiculos y llama a la opcion elegida
// Se repite hasta que el usuario elija 0 (volver)
void menuVehiculos(SistemaTaller* s) {
    int opcion = -1;
    do {
        cout << "\n--- MENU VEHICULOS ---\n";
        cout << "1. Registrar vehiculo\n";
        cout << "2. Buscar vehiculo\n";
        cout << "3. Actualizar vehiculo\n";
        cout << "4. Eliminar vehiculo\n";
        cout << "5. Listar vehiculos\n";
        cout << "0. Volver\n";
        opcion = leerOpcion("Seleccione una opcion", 0, 5);

        switch (opcion) {
            case 1:
                menuRegistrarVehiculo(s);
                break;
            case 2:
                menuBuscarVehiculo(s);
                break;
            case 3:
                menuActualizarVehiculo(s);
                break;
            case 4:
                menuEliminarVehiculo(s);
                break;
            case 5:
                menuListarVehiculos(s);
                break;
            case 0:
                cout << "Regresando al menu principal...\n";
                break;
        }
    } while (opcion != 0 && opcion != -1); // -1: se acabo la entrada
}

// Pide los datos del vehiculo, pide confirmacion y llama a agregarVehiculo
// Si en cualquier dato escriben CANCELAR, se sale sin guardar (caso de uso 8)
void menuRegistrarVehiculo(SistemaTaller* s) {
    cout << "\n--- REGISTRAR VEHICULO (Escriba CANCELAR para abortar) ---\n";

    Vehiculo datos;
    if (!leerTexto("Placa: ", datos.placa, 10)) return;
    if (!leerTexto("Marca: ", datos.marca, 50)) return;
    if (!leerTexto("Modelo: ", datos.modelo, 50)) return;
    if (!leerEntero("Anio: ", &datos.anio)) return;
    if (!leerEntero("Kilometraje: ", &datos.kilometraje)) return;
    if (!leerTexto("Propietario: ", datos.nombrePropietario, 100)) return;
    if (!leerTexto("Cedula del propietario: ", datos.cedulaPropietario, 20)) return;
    if (!leerTexto("Telefono del propietario: ", datos.telefonoPropietario, 20)) return;

    if (!confirmar("Desea guardar este vehiculo? (s/n): ")) {
        cout << "Operacion cancelada por el usuario.\n";
        return;
    }

    Vehiculo* nuevo = agregarVehiculo(s, datos);
    if (nuevo == nullptr) {
        cout << "ERROR: La placa " << datos.placa << " ya esta registrada o algun dato no es valido.\n";
    } else {
        cout << "Vehiculo registrado exitosamente con ID " << nuevo->id << ".\n";
    }
}

// Busca por placa exacta o por parte del nombre del propietario
// Con buscarVehiculosPorPropietario, al final hacer delete[] del arreglo
void menuBuscarVehiculo(SistemaTaller* s) {
    cout << "\n--- BUSCAR VEHICULO ---\n";
    cout << "1. Buscar por placa exacta\n";
    cout << "2. Buscar por propietario (parcial)\n";
    int op = leerOpcion("Seleccione opcion", 1, 2);

    if (op == 1) {
        char placa[10];
        if (!leerTexto("Ingrese placa: ", placa, 10)) return;
        convertirAMayusculas(placa); // las placas se guardan en mayusculas
        Vehiculo* v = buscarVehiculoPorPlaca(s, placa);
        if (v != nullptr) {
            mostrarVehiculo(v);
        } else {
            cout << "No se encontro ningun vehiculo con la placa: " << placa << "\n";
        }
    } else if (op == 2) {
        char prop[100];
        if (!leerTexto("Ingrese nombre o parte del propietario: ", prop, 100)) return;
        int cantidad = 0;
        Vehiculo** lista = buscarVehiculosPorPropietario(s, prop, &cantidad);
        if (cantidad > 0) {
            mostrarListaVehiculos(lista, cantidad);
        } else {
            cout << "No se encontraron vehiculos para ese propietario.\n";
        }
        delete[] lista;
    }
}

// Pide el ID, muestra el vehiculo, pide los datos nuevos y confirmacion,
// y llama a actualizarVehiculo
void menuActualizarVehiculo(SistemaTaller* s) {
    cout << "\n--- ACTUALIZAR VEHICULO ---\n";
    int id;
    if (!leerEntero("Ingrese el ID del vehiculo a actualizar: ", &id)) return;

    Vehiculo* actual = buscarVehiculoPorID(s, id);
    if (actual == nullptr) {
        cout << "Vehiculo no encontrado.\n";
        return;
    }

    cout << "\nVehiculo actual:\n";
    mostrarVehiculo(actual);

    cout << "\n--- Ingrese nuevos datos (o CANCELAR para salir) ---\n";
    Vehiculo datos;
    if (!leerTexto("Nueva Marca: ", datos.marca, 50)) return;
    if (!leerTexto("Nuevo Modelo: ", datos.modelo, 50)) return;
    if (!leerEntero("Nuevo Anio: ", &datos.anio)) return;
    if (!leerTexto("Nuevo Propietario: ", datos.nombrePropietario, 100)) return;
    if (!leerTexto("Nueva Cedula del propietario: ", datos.cedulaPropietario, 20)) return;
    if (!leerTexto("Nuevo Telefono del propietario: ", datos.telefonoPropietario, 20)) return;

    if (!confirmar("Desea guardar los cambios? (s/n): ")) {
        cout << "Actualizacion cancelada.\n";
        return;
    }

    bool exito = actualizarVehiculo(s, id, datos);
    if (exito) {
        cout << "Vehiculo actualizado correctamente.\n";
    } else {
        cout << "Error: algun dato no es valido (anio entre 1950 y 2027, cedula valida).\n";
    }
}

// Pide el ID, pide confirmacion y llama a eliminarVehiculo
// Si tiene ordenes, el mensaje debe decir cuantas (contarOrdenesDeVehiculo)
void menuEliminarVehiculo(SistemaTaller* s) {
    cout << "\n--- ELIMINAR VEHICULO ---\n";
    int id;
    if (!leerEntero("Ingrese el ID del vehiculo a eliminar: ", &id)) return;

    // eliminarVehiculo devuelve bool, asi que primero se revisa por que podria fallar
    Vehiculo* v = buscarVehiculoPorID(s, id);
    if (v == nullptr) {
        cout << "Vehiculo no encontrado.\n";
        return;
    }

    int ordenes = contarOrdenesDeVehiculo(s, id);
    if (ordenes > 0) {
        cout << "No se puede eliminar: este vehiculo tiene " << ordenes << " orden(es) asociada(s).\n";
        return;
    }

    mostrarVehiculo(v);

    if (!confirmar("Esta seguro que desea eliminar este vehiculo? (s/n): ")) {
        cout << "Eliminacion cancelada.\n";
        return;
    }

    bool eliminado = eliminarVehiculo(s, id);
    if (eliminado) {
        cout << "Vehiculo eliminado con exito.\n";
    } else {
        cout << "No se pudo eliminar el vehiculo.\n";
    }
}

// Llama a listarVehiculos, muestra la lista y hace delete[] del arreglo
void menuListarVehiculos(SistemaTaller* s) {
    cout << "\n--- LISTA DE VEHICULOS ---\n";
    int cantidad = 0;
    Vehiculo** lista = listarVehiculos(s, &cantidad);

    if (cantidad > 0) {
        mostrarListaVehiculos(lista, cantidad);
    } else {
        cout << "No hay vehiculos registrados en el sistema.\n";
    }

    delete[] lista;
}

// Muestra los datos de un solo vehiculo
void mostrarVehiculo(Vehiculo* vehiculo) {
    if (vehiculo == nullptr) return;
    cout << "----------------------------------------\n";
    cout << "ID: " << vehiculo->id << "\n";
    cout << "Placa: " << vehiculo->placa << "\n";
    cout << "Marca: " << vehiculo->marca << "\n";
    cout << "Modelo: " << vehiculo->modelo << "\n";
    cout << "Anio: " << vehiculo->anio << "\n";
    cout << "Kilometraje: " << vehiculo->kilometraje << "\n";
    cout << "Propietario: " << vehiculo->nombrePropietario << "\n";
    cout << "Cedula: " << vehiculo->cedulaPropietario << "\n";
    cout << "Telefono: " << vehiculo->telefonoPropietario << "\n";
    cout << "Registrado: " << vehiculo->fechaRegistro << "\n";
    cout << "----------------------------------------\n";
}

// Muestra una lista de vehiculos en forma de tabla
void mostrarListaVehiculos(Vehiculo** vehiculos, int cantidad) {
    if (vehiculos == nullptr || cantidad <= 0) return;

    cout << left
         << setw(5) << "ID"
         << setw(10) << "PLACA"
         << setw(15) << "MARCA"
         << setw(15) << "MODELO"
         << setw(8) << "ANIO"
         << setw(10) << "KM"
         << setw(20) << "PROPIETARIO" << "\n";
    cout << string(83, '-') << "\n";

    for (int i = 0; i < cantidad; i++) {
        if (vehiculos[i] != nullptr) {
            cout << left
                 << setw(5) << vehiculos[i]->id
                 << setw(10) << vehiculos[i]->placa
                 << setw(15) << vehiculos[i]->marca
                 << setw(15) << vehiculos[i]->modelo
                 << setw(8) << vehiculos[i]->anio
                 << setw(10) << vehiculos[i]->kilometraje
                 << setw(20) << vehiculos[i]->nombrePropietario << "\n";
        }
    }
    cout << "Total de vehiculos: " << cantidad << "\n";
}
