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
        cout << "Seleccione una opcion: ";
        cin >> opcion;

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
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);
}

// Pide nombre, cedula, especialidad y fecha de ingreso, pide confirmacion
// y llama a registrarMecanico. Segun el Resultado, muestra el mensaje:
// ERROR_DUPLICADO -> cedula repetida, ERROR_DATO_INVALIDO -> dato fuera de lo permitido
void menuRegistrarMecanico(SistemaTaller* s) {
    string nombre, cedula, especialidad, fechaIngreso;
    
    cout << "\n--- REGISTRAR MECANICO ---" << endl;
    cout << "Ingrese Nombre: ";
    cin.ignore(); // Limpia el buffer antes de leer strings con espacios
    getline(cin, nombre);
    cout << "Ingrese Cedula: ";
    getline(cin, cedula);
    cout << "Ingrese Especialidad: ";
    getline(cin, especialidad);
    cout << "Ingrese Fecha de Ingreso (DD/MM/AAAA): ";
    getline(cin, fechaIngreso);

    char confirmacion;
    cout << "¿Confirmar registro del mecanico? (S/N): ";
    cin >> confirmacion;

    if (confirmacion == 'S' || confirmacion == 's') {
        // Se asume que registrarMecanico devuelve un enum Resultado
        Resultado res = registrarMecanico(s, nombre, cedula, especialidad, fechaIngreso);

        if (res == EXITO) {
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
    cout << "Ingrese el ID del mecanico: ";
    cin >> id;

    // Se asume que devuelve un puntero a Mecanico o NULL si no existe
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
    // Se asume que listarMecanicos devuelve un arreglo dinámico de punteros (Mecanico**)
    // y modifica la variable 'cantidad' por referencia o puntero.
    Mecanico** lista = listarMecanicos(s, &cantidad);

    if (lista != nullptr && cantidad > 0) {
        mostrarListaMecanicos(s, lista, cantidad);
        
        // Liberación de la memoria del arreglo dinámico devuelto
        delete[] lista; 
    } else {
        cout << "No hay mecanicos registrados en el sistema." << endl;
    }
}

// Muestra un mecanico con su numero de ordenes ABIERTAS (contarOrdenesAbiertasMecanico)
void mostrarMecanico(SistemaTaller* s, Mecanico* mecanico) {
    if (mecanico == nullptr) return;

    // Se asume que la estructura Mecanico tiene estos campos accesibles (o mediante getters)
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
