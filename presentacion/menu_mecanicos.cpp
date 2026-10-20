#include "menu_mecanicos.h"
#include "entrada.h"
#include "../logica/mecanicos.h"
#include <iostream>

using namespace std;

// Muestra el submenu de mecanicos. Los mecanicos no se actualizan ni se eliminan
void menuMecanicos(SistemaTaller* s) {
    int opcion;
    do {
        cout << "\n--- MENU MECANICOS ---" << endl;
        cout << "1. Registrar Mecanico" << endl;
        cout << "2. Buscar Mecanico por ID" << endl;
        cout << "3. Listar Mecanicos" << endl;
        cout << "0. Volver" << endl;
        opcion = leerOpcion("Seleccione una opcion", 0, 3);

        switch (opcion) {
            case 1:
                menuRegistrarMecanico(s);
                break;
            case 2:
                menuBuscarMecanico(s);
                break;
            case 3:
                menuListarMecanicos(s);
                break;
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
        }
    } while (opcion != 0 && opcion != -1); // -1: se acabo la entrada
}

// Pide nombre, cedula, especialidad y fecha de ingreso, pide confirmacion
// y llama a registrarMecanico. Segun el Resultado, muestra el mensaje:
// ERROR_DUPLICADO -> cedula repetida, ERROR_DATO_INVALIDO -> dato fuera de lo permitido
void menuRegistrarMecanico(SistemaTaller* s) {
    Mecanico datos;

    cout << "\n--- REGISTRAR MECANICO ---" << endl;
    cout << "(Escriba CANCELAR en cualquier momento para salir)" << endl;
    if (!leerTexto("Ingrese Nombre: ", datos.nombre, 100)) return;
    if (!leerTexto("Ingrese Cedula: ", datos.cedula, 20)) return;
    if (!leerTexto("Ingrese Especialidad (MOTOR, FRENOS, ELECTRICIDAD, SUSPENSION, GENERAL): ", datos.especialidad, 20)) return;
    if (!leerTexto("Ingrese Fecha de Ingreso (AAAA-MM-DD): ", datos.fechaIngreso, 11)) return;

    if (confirmar("Confirmar registro del mecanico? (S/N): ")) {
        Resultado res = registrarMecanico(s, datos);

        if (res == OK) {
            cout << "Mecanico registrado con exito." << endl;
        } else if (res == ERROR_DUPLICADO) {
            cout << "Error: La cedula ya se encuentra registrada en el sistema." << endl;
        } else if (res == ERROR_DATO_INVALIDO) {
            cout << "Error: Uno o mas datos ingresados no son validos." << endl;
        } else {
            cout << "Error desconocido al registrar." << endl;
        }
    } else {
        cout << "Registro cancelado por el usuario." << endl;
    }
}

// Pide el ID y muestra el mecanico con buscarMecanicoPorID
void menuBuscarMecanico(SistemaTaller* s) {
    int id;
    cout << "\n--- BUSCAR MECANICO ---" << endl;
    if (!leerEntero("Ingrese el ID del mecanico: ", &id)) return;

    Mecanico* mecanico = buscarMecanicoPorID(s, id);

    if (mecanico != nullptr) {
        mostrarMecanico(s, mecanico);
    } else {
        cout << "No se encontro ningun mecanico con el ID especificado." << endl;
    }
}

// Llama a listarMecanicos, muestra la lista y hace delete[] del arreglo
void menuListarMecanicos(SistemaTaller* s) {
    int cantidad = 0;
    Mecanico** lista = listarMecanicos(s, &cantidad);

    if (lista != nullptr && cantidad > 0) {
        mostrarListaMecanicos(s, lista, cantidad);
    } else {
        cout << "No hay mecanicos registrados en el sistema." << endl;
    }
    delete[] lista; // se libera aunque venga vacio
}

// Muestra un mecanico con su numero de ordenes ABIERTAS (contarOrdenesAbiertasMecanico)
void mostrarMecanico(SistemaTaller* s, Mecanico* mecanico) {
    if (mecanico == nullptr) return;

    cout << "\nID: " << mecanico->id << endl;
    cout << "Nombre: " << mecanico->nombre << endl;
    cout << "Cedula: " << mecanico->cedula << endl;
    cout << "Especialidad: " << mecanico->especialidad << endl;
    cout << "Fecha de Ingreso: " << mecanico->fechaIngreso << endl;
    
    // Llamada a la función lógica para contar órdenes abiertas
    int ordenesAbiertas = contarOrdenesAbiertasMecanico(s, mecanico->id);
    cout << "Ordenes Abiertas: " << ordenesAbiertas << endl;
    cout << "-----------------------------------" << endl;
}

// Muestra la lista de mecanicos; cada uno con sus ordenes ABIERTAS
void mostrarListaMecanicos(SistemaTaller* s, Mecanico** mecanicos, int cantidad) {
    cout << "\n--- LISTA DE MECANICOS (" << cantidad << ") ---" << endl;
    for (int i = 0; i < cantidad; i++) {
        mostrarMecanico(s, mecanicos[i]);
    }
}
