#include "menu_repuestos.h"
#include "entrada.h"
#include "../logica/repuestos.h"
#include <iostream>

using namespace std;

// Muestra el submenu de inventario
void menuRepuestos(SistemaTaller* s) {
    int opcion = -1;
    do {
        cout << "\n--- MENU REPUESTOS ---" << endl;
        cout << "1. Registrar repuesto" << endl;
        cout << "2. Buscar repuesto" << endl;
        cout << "3. Entrada de mercancia" << endl;
        cout << "4. Cambiar precio" << endl;
        cout << "5. Eliminar repuesto" << endl;
        cout << "6. Listar repuestos" << endl;
        cout << "0. Volver" << endl;
        opcion = leerOpcion("Seleccione una opcion", 0, 6);

        switch (opcion) {
            case 1:
                menuRegistrarRepuesto(s);
                break;
            case 2:
                menuBuscarRepuesto(s);
                break;
            case 3:
                menuEntradaMercancia(s);
                break;
            case 4:
                menuCambiarPrecio(s);
                break;
            case 5:
                menuEliminarRepuesto(s);
                break;
            case 6:
                menuListarRepuestos(s);
                break;
            case 0:
                cout << "Volviendo..." << endl;
                break;
        }
    } while (opcion != 0 && opcion != -1); // -1: se acabo la entrada
}

// Pide codigo, nombre, marca, precio, stock y stock minimo, pide confirmacion
// y llama a registrarRepuesto. Mostrar el mensaje segun el Resultado
void menuRegistrarRepuesto(SistemaTaller* s) {
    Repuesto datos;

    cout << "\n--- REGISTRAR REPUESTO ---" << endl;
    cout << "(Escriba CANCELAR en cualquier momento para salir)" << endl;
    if (!leerTexto("Codigo: ", datos.codigo, 20)) return;
    if (!leerTexto("Nombre: ", datos.nombre, 100)) return;
    if (!leerTexto("Marca: ", datos.marca, 50)) return;
    if (!leerFlotante("Precio: ", &datos.precio)) return;
    if (!leerEntero("Stock: ", &datos.stock)) return;
    if (!leerEntero("Stock minimo: ", &datos.stockMinimo)) return;

    if (confirmar("Confirma el registro? (s/n): ")) {
        Resultado res = registrarRepuesto(s, datos);
        if (res == OK) {
            cout << "Repuesto registrado exitosamente." << endl;
        } else if (res == ERROR_DUPLICADO) {
            cout << "Error: Ya existe un repuesto con ese codigo." << endl;
        } else if (res == ERROR_DATO_INVALIDO) {
            cout << "Error: Revise los datos (precio mayor a 0, stock no negativo)." << endl;
        } else {
            cout << "Error al registrar el repuesto." << endl;
        }
    } else {
        cout << "Operacion cancelada." << endl;
    }
}

// Busca por codigo o por parte del nombre con buscarRepuestos
// Al final, delete[] del arreglo
void menuBuscarRepuesto(SistemaTaller* s) {
    char termino[100];
    cout << "\n--- BUSCAR REPUESTO ---" << endl;
    if (!leerTexto("Ingrese codigo o nombre: ", termino, 100)) return;

    int cantidad = 0;
    Repuesto** encontrados = buscarRepuestos(s, termino, &cantidad);
    mostrarListaRepuestos(encontrados, cantidad);

    delete[] encontrados;
}

// Pide el ID y la cantidad que llego, y llama a registrarEntradaMercancia
void menuEntradaMercancia(SistemaTaller* s) {
    int id, cantidad;
    cout << "\n--- ENTRADA DE MERCANCIA ---" << endl;
    if (!leerEntero("ID del repuesto: ", &id)) return;
    if (!leerEntero("Cantidad que llego: ", &cantidad)) return;

    Resultado res = registrarEntradaMercancia(s, id, cantidad);
    if (res == OK) {
        cout << "Entrada de mercancia registrada con exito." << endl;
    } else if (res == ERROR_NO_EXISTE) {
        cout << "Error: No existe un repuesto con ese ID." << endl;
    } else if (res == ERROR_DATO_INVALIDO) {
        cout << "Error: La cantidad debe ser mayor a 0." << endl;
    } else {
        cout << "Error al registrar la entrada de mercancia." << endl;
    }
}

// Pide el ID y el precio nuevo, y llama a cambiarPrecioRepuesto
void menuCambiarPrecio(SistemaTaller* s) {
    int id;
    float nuevoPrecio;
    cout << "\n--- CAMBIAR PRECIO ---" << endl;
    if (!leerEntero("ID del repuesto: ", &id)) return;
    if (!leerFlotante("Nuevo precio: ", &nuevoPrecio)) return;

    Resultado res = cambiarPrecioRepuesto(s, id, nuevoPrecio);
    if (res == OK) {
        cout << "Precio actualizado correctamente." << endl;
    } else if (res == ERROR_NO_EXISTE) {
        cout << "Error: No existe un repuesto con ese ID." << endl;
    } else if (res == ERROR_DATO_INVALIDO) {
        cout << "Error: El precio debe ser mayor a 0." << endl;
    } else {
        cout << "Error al cambiar el precio." << endl;
    }
}

// Pide el ID, pide confirmacion y llama a eliminarRepuesto
// ERROR_EN_USO -> el repuesto aparece en alguna orden
void menuEliminarRepuesto(SistemaTaller* s) {
    int id;
    cout << "\n--- ELIMINAR REPUESTO ---" << endl;
    if (!leerEntero("ID del repuesto: ", &id)) return;

    if (confirmar("Esta seguro que desea eliminar el repuesto? (s/n): ")) {
        Resultado res = eliminarRepuesto(s, id);
        if (res == OK) {
            cout << "Repuesto eliminado correctamente." << endl;
        } else if (res == ERROR_NO_EXISTE) {
            cout << "Error: No existe un repuesto con ese ID." << endl;
        } else if (res == ERROR_EN_USO) {
            cout << "Error: El repuesto aparece en alguna orden." << endl;
        } else {
            cout << "Error al eliminar el repuesto." << endl;
        }
    } else {
        cout << "Operacion cancelada." << endl;
    }
}

// Llama a listarRepuestos, muestra la lista y hace delete[] del arreglo
void menuListarRepuestos(SistemaTaller* s) {
    int cantidad = 0;
    Repuesto** lista = listarRepuestos(s, &cantidad);
    mostrarListaRepuestos(lista, cantidad);
    delete[] lista;
}

// Muestra los datos de un repuesto
void mostrarRepuesto(Repuesto* repuesto) {
    if (repuesto == nullptr) return;
    cout << "ID: " << repuesto->id
         << " | Codigo: " << repuesto->codigo
         << " | Nombre: " << repuesto->nombre
         << " | Marca: " << repuesto->marca
         << " | Precio: " << repuesto->precio
         << " | Stock: " << repuesto->stock;

    if (tieneBajoStock(repuesto)) {
        cout << " [BAJO STOCK]";
    }
    cout << endl;
}

// Muestra la lista de repuestos. Los que cumplan tieneBajoStock
// se marcan con BAJO STOCK (caso de uso 2)
void mostrarListaRepuestos(Repuesto** repuestos, int cantidad) {
    if (cantidad <= 0 || repuestos == nullptr) {
        cout << "No hay repuestos para mostrar." << endl;
        return;
    }
    cout << "\n--- LISTA DE REPUESTOS (" << cantidad << ") ---" << endl;
    for (int i = 0; i < cantidad; i++) {
        cout << i + 1 << ". ";
        mostrarRepuesto(repuestos[i]);
    }
}
