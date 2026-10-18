#include "menu_vehiculos.h"
#include "entrada.h"
#include "../logica/vehiculos.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Muestra el submenu de vehiculos y llama a la opcion elegida
// Se repite hasta que el usuario elija 0 (volver)
void menuVehiculos(SistemaTaller* s) {
    int opcion = -1;
    do {
        cout << "\n===== MENU VEHICULOS =====\n";
        cout << "1. Registrar vehiculo\n";
        cout << "2. Buscar vehiculo\n";
        cout << "3. Actualizar vehiculo\n";
        cout << "4. Eliminar vehiculo\n";
        cout << "5. Listar vehiculos\n";
        cout << "0. Volver\n";
        cout << "Seleccione una opcion: ";
        
        cin >> opcion;
        limpiarBuffer(); // Suponiendo que existe en entrada.h

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
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (opcion != 0);
}

// Pide los datos del vehiculo, pide confirmacion y llama a agregarVehiculo
// Si en cualquier dato escriben CANCELAR, se sale sin guardar (caso de uso 8)
void menuRegistrarVehiculo(SistemaTaller* s) {
    cout << "\n--- REGISTRAR VEHICULO (Escriba CANCELAR para abortar) ---\n";
    
    string placa, marca, modelo, propietario;
    int anio;

    cout << "Placa: ";
    placa = leerCadena();
    if (placa == "CANCELAR") return;

    cout << "Marca: ";
    marca = leerCadena();
    if (marca == "CANCELAR") return;

    cout << "Modelo: ";
    modelo = leerCadena();
    if (modelo == "CANCELAR") return;

    cout << "Anio: ";
    anio = leerEntero();
    if (anio == -1 /* o valor cancelado */) return;

    cout << "Propietario: ";
    propietario = leerCadena();
    if (propietario == "CANCELAR") return;

    if (!confirmar("¿Desea guardar este vehiculo? (s/n): ")) {
        cout << "Operacion cancelada por el usuario.\n";
        return;
    }

    Vehiculo* nuevo = agregarVehiculo(s, placa, marca, modelo, anio, propietario);
    if (nuevo == nullptr) {
        cout << "ERROR: La placa " << placa << " ya esta registrada o hubo un fallo al guardar.\n";
    } else {
        cout << "Vehiculo registrado exitosamente.\n";
    }
}

// Busca por placa exacta o por parte del nombre del propietario
// Con buscarVehiculosPorPropietario, al final hacer delete[] del arreglo
void menuBuscarVehiculo(SistemaTaller* s) {
    cout << "\n--- BUSCAR VEHICULO ---\n";
    cout << "1. Buscar por placa exacta\n";
    cout << "2. Buscar por propietario (parcial)\n";
    cout << "Seleccione opcion: ";
    
    int op = leerEntero();
    if (op == 1) {
        cout << "Ingrese placa: ";
        string placa = leerCadena();
        Vehiculo* v = buscarVehiculoPorPlaca(s, placa); // Asumiendo esta funcion
        if (v != nullptr) {
            mostrarVehiculo(v);
        } else {
            cout << "No se encontro ningun vehiculo con la placa: " << placa << "\n";
        }
    } else if (op == 2) {
        cout << "Ingrese nombre o parte del propietario: ";
        string prop = leerCadena();
        int cantidad = 0;
        Vehiculo** lista = buscarVehiculosPorPropietario(s, prop, cantidad);
        if (cantidad > 0) {
            mostrarListaVehiculos(lista, cantidad);
        } else {
            cout << "No se encontraron vehiculos para ese propietario.\n";
        }
        delete[] lista;
    } else {
        cout << "Opcion invalida.\n";
    }
}

// Pide el ID, muestra el vehiculo, pide los datos nuevos y confirmacion,
// y llama a actualizarVehiculo
void menuActualizarVehiculo(SistemaTaller* s) {
    cout << "\n--- ACTUALIZAR VEHICULO ---\n";
    cout << "Ingrese ID o Placa del vehiculo a actualizar: "; // Ajustar segun tu modelo (ID o Placa)
    string idVehiculo = leerCadena();

    Vehiculo* actual = buscarVehiculoPorPlaca(s, idVehiculo); // o por ID
    if (actual == nullptr) {
        cout << "Vehiculo no encontrado.\n";
        return;
    }

    cout << "\nVehiculo actual:\n";
    mostrarVehiculo(actual);

    cout << "\n--- Ingrese nuevos datos (o CANCELAR para salir) ---\n";
    cout << "Nueva Marca: ";
    string marca = leerCadena();
    if (marca == "CANCELAR") return;

    cout << "Nuevo Modelo: ";
    string modelo = leerCadena();
    if (modelo == "CANCELAR") return;

    cout << "Nuevo Anio: ";
    int anio = leerEntero();

    cout << "Nuevo Propietario: ";
    string propietario = leerCadena();
    if (propietario == "CANCELAR") return;

    if (!confirmar("¿Desea guardar los cambios? (s/n): ")) {
        cout << "Actualizacion cancelada.\n";
        return;
    }

    bool exito = actualizarVehiculo(s, idVehiculo, marca, modelo, anio, propietario);
    if (exito) {
        cout << "Vehiculo actualizado correctamente.\n";
    } else {
        cout << "Error al actualizar el vehiculo.\n";
    }
}

// Pide el ID, pide confirmacion y llama a eliminarVehiculo
// Si tiene ordenes, el mensaje debe decir cuantas (contarOrdenesDeVehiculo)
void menuEliminarVehiculo(SistemaTaller* s) {
    cout << "\n--- ELIMINAR VEHICULO ---\n";
    cout << "Ingrese placa o ID del vehiculo a eliminar: ";
    string idVehiculo = leerCadena();

    Vehiculo* v = buscarVehiculoPorPlaca(s, idVehiculo);
    if (v == nullptr) {
        cout << "Vehiculo no encontrado.\n";
        return;
    }

    int ordenes = contarOrdenesDeVehiculo(s, idVehiculo);
    if (ordenes > 0) {
        cout << "ADVERTENCIA: Este vehiculo tiene " << ordenes << " orden(es) asociada(s).\n";
    }

    mostrarVehiculo(v);

    if (!confirmar("¿Esta seguro que desea eliminar este vehiculo? (s/n): ")) {
        cout << "Eliminacion cancelada.\n";
        return;
    }

    bool eliminado = eliminarVehiculo(s, idVehiculo);
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
    Vehiculo** lista = listarVehiculos(s, cantidad);
    
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
    cout << "Placa:       " << vehiculo->getPlaca() << "\n"; // Ajustar a campos/getters reales
    cout << "Marca:       " << vehiculo->getMarca() << "\n";
    cout << "Modelo:      " << vehiculo->getModelo() << "\n";
    cout << "Anio:        " << vehiculo->getAnio() << "\n";
    cout << "Propietario: " << vehiculo->getPropietario() << "\n";
    cout << "----------------------------------------\n";
}

// Muestra una lista de vehiculos en forma de tabla
void mostrarListaVehiculos(Vehiculo** vehiculos, int cantidad) {
    if (vehiculos == nullptr || cantidad <= 0) return;

    cout << left 
         << setw(12) << "PLACA" 
         << setw(15) << "MARCA" 
         << setw(15) << "MODELO" 
         << setw(8)  << "ANIO" 
         << setw(20) << "PROPIETARIO" << "\n";
    cout << string(70, '-') << "\n";

    for (int i = 0; i < cantidad; i++) {
        if (vehiculos[i] != nullptr) {
            cout << left 
                 << setw(12) << vehiculos[i]->getPlaca()
                 << setw(15) << vehiculos[i]->getMarca()
                 << setw(15) << vehiculos[i]->getModelo()
                 << setw(8)  << vehiculos[i]->getAnio()
                 << setw(20) << vehiculos[i]->getPropietario() << "\n";
        }
    }
}
