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
        cout << "Seleccione una opcion: ";
        cin >> opcion;

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
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);
}

// Pide codigo, nombre, marca, precio, stock y stock minimo, pide confirmacion
// y llama a registrarRepuesto. Mostrar el mensaje segun el Resultado
void menuRegistrarRepuesto(SistemaTaller* s) {
    string codigo, nombre, marca;
    double precio;
    int stock, stockMinimo;

    cout << "\n--- REGISTRAR REPUESTO ---" << endl;
    cout << "Codigo: ";
    cin >> codigo;
    cout << "Nombre: ";
    cin >> nombre;
    cout << "Marca: ";
    cin >> marca;
    cout << "Precio: ";
    cin >> precio;
    cout << "Stock: ";
    cin >> stock;
    cout << "Stock minimo: ";
    cin >> stockMinimo;

    char confirma;
    cout << "Confirma el registro? (s/n): ";
    cin >> confirma;

    if (confirma == 's' || confirma == 'S') {
        Resultado res = s->registrarRepuesto(codigo, nombre, marca, precio, stock, stockMinimo);
        if (res == EXITO) {
            cout << "Repuesto registrado exitosamente." << endl;
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
    string termino;
    cout << "\n--- BUSCAR REPUESTO ---" << endl;
    cout << "Ingrese codigo o nombre: ";
    cin >> termino;

    int cantidad = 0;
    Repuesto** encontrados = s->buscarRepuestos(termino, cantidad);
    mostrarListaRepuestos(encontrados, cantidad);

    delete[] encontrados;
}

// Pide el ID y la cantidad que llego, y llama a registrarEntradaMercancia
void menuEntradaMercancia(SistemaTaller* s) {
    string id;
    int cantidad;
    cout << "\n--- ENTRADA DE MERCANCIA ---" << endl;
    cout << "ID del repuesto: ";
    cin >> id;
    cout << "Cantidad que llego: ";
    cin >> cantidad;

    Resultado res = s->registrarEntradaMercancia(id, cantidad);
    if (res == EXITO) {
        cout << "Entrada de mercancia registrada con exito." << endl;
    } else {
        cout << "Error al registrar la entrada de mercancia." << endl;
    }
}

// Pide el ID y el precio nuevo, y llama a cambiarPrecioRepuesto
void menuCambiarPrecio(SistemaTaller* s) {
    string id;
    double nuevoPrecio;
    cout << "\n--- CAMBIAR PRECIO ---" << endl;
    cout << "ID del repuesto: ";
    cin >> id;
    cout << "Nuevo precio: ";
    cin >> nuevoPrecio;

    Resultado res = s->cambiarPrecioRepuesto(id, nuevoPrecio);
    if (res == EXITO) {
        cout << "Precio actualizado correctamente." << endl;
    } else {
        cout << "Error al cambiar el precio." << endl;
    }
}

// Pide el ID, pide confirmacion y llama a eliminarRepuesto
// ERROR_EN_USO -> el repuesto aparece en alguna orden
void menuEliminarRepuesto(SistemaTaller* s) {
    string id;
    cout << "\n--- ELIMINAR REPUESTO ---" << endl;
    cout << "ID del repuesto: ";
    cin >> id;

    char confirma;
    cout << "Esta seguro que desea eliminar el repuesto? (s/n): ";
    cin >> confirma;

    if (confirma == 's' || confirma == 'S') {
        Resultado res = s->eliminarRepuesto(id);
        if (res == EXITO) {
            cout << "Repuesto eliminado correctamente." << endl;
        } else if (res == ERROR_EN_USO) {
            cout << "Error: El repuesto aparece en alguna orden activa." << endl;
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
    Repuesto** lista = s->listarRepuestos(cantidad);
    mostrarListaRepuestos(lista, cantidad);
    delete[] lista;
}

// Muestra los datos de un repuesto
void mostrarRepuesto(Repuesto* repuesto) {
    if (repuesto == nullptr) return;
    cout << "Codigo: " << repuesto->getCodigo()
         << " | Nombre: " << repuesto->getNombre()
         << " | Marca: " << repuesto->getMarca()
         << " | Precio: " << repuesto->getPrecio()
         << " | Stock: " << repuesto->getStock();
    
    if (repuesto->tieneBajoStock()) {
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
