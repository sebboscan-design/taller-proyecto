#include "menu_ordenes.h"
#include "entrada.h"
#include "../logica/ordenes.h"
#include "../logica/vehiculos.h"
#include "../logica/mecanicos.h"
#include "../logica/repuestos.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Muestra el submenu de ordenes de servicio
void menuOrdenes(SistemaTaller* s) {
    int opcion = -1;
    do {
        cout << "\n=== MENU ORDENES DE SERVICIO ===\n"
             << "1. Abrir orden de servicio\n"
             << "2. Agregar repuesto a orden\n"
             << "3. Quitar repuesto de orden\n"
             << "4. Cerrar orden de servicio\n"
             << "5. Anular orden de servicio\n"
             << "6. Ver detalle de orden\n"
             << "7. Listar ordenes\n"
             << "0. Volver al menu principal\n"
             << "Seleccione una opcion: ";
        opcion = leerEntero();

        switch (opcion) {
            case 1: menuAbrirOrden(s); break;
            case 2: menuAgregarRepuestoOrden(s); break;
            case 3: menuQuitarRepuestoOrden(s); break;
            case 4: menuCerrarOrden(s); break;
            case 5: menuAnularOrden(s); break;
            case 6: menuVerDetalleOrden(s); break;
            case 7: menuListarOrdenes(s); break;
            case 0: cout << "Regresando...\n"; break;
            default: cout << "Opcion invalida.\n"; break;
        }
    } while (opcion != 0);
}

// Pide placa del vehiculo, ID del mecanico, kilometraje y falla,
// pide confirmacion y llama a abrirOrden.
void menuAbrirOrden(SistemaTaller* s) {
    cout << "\n--- ABRIR ORDEN DE SERVICIO ---\n";
    string placa = leerCadena("Placa del vehiculo: ");
    int idMecanico = leerEntero("ID del mecanico: ");
    double kilometraje = leerDouble("Kilometraje actual: ");
    string falla = leerCadena("Descripcion de la falla: ");

    char conf = leerChar("¿Confirma la apertura de la orden? (S/N): ");
    if (conf != 'S' && conf != 's') {
        cout << "Operacion cancelada.\n";
        return;
    }

    ResultadoOrden res = abrirOrden(s, placa.c_str(), idMecanico, kilometraje, falla.c_str());

    if (res.exito) {
        cout << "Orden #" << res.idOrden << " abierta exitosamente.\n";
    } else {
        switch (res.codigoError) {
            case ERROR_VEHICULO_CON_ORDEN_ABIERTA:
                cout << "Error: El vehiculo " << placa << " ya tiene una orden abierta.\n";
                break;
            case ERROR_MECANICO_SIN_CUPO:
                cout << "Error: El mecanico seleccionado ya no tiene cupo de ordenes abiertas.\n";
                break;
            case ERROR_KILOMETRAJE_MENOR:
                cout << "Error: El kilometraje es menor al ultimo registrado en el vehiculo.\n";
                break;
            default:
                cout << "Error al abrir la orden.\n";
                break;
        }
    }
}

// Pide la orden, el repuesto y la cantidad, y llama a agregarRepuestoAOrden
void menuAgregarRepuestoOrden(SistemaTaller* s) {
    cout << "\n--- AGREGAR REPUESTO A ORDEN ---\n";
    int idOrden = leerEntero("Numero de orden: ");
    int idRepuesto = leerEntero("ID del repuesto: ");
    int cantidad = leerEntero("Cantidad: ");

    ResultadoOrden res = agregarRepuestoAOrden(s, idOrden, idRepuesto, cantidad);

    if (res.exito) {
        cout << "Repuesto agregado correctamente a la orden #" << idOrden << ".\n";
    } else {
        switch (res.codigoError) {
            case ERROR_STOCK_INSUFICIENTE:
                cout << "Error: Stock insuficiente para el repuesto solicitado.\n";
                break;
            case ERROR_ESTADO_INVALIDO:
                cout << "Error: La orden #" << idOrden << " esta CERRADA o ANULADA y no puede modificarse.\n";
                break;
            default:
                cout << "Error al agregar repuesto.\n";
                break;
        }
    }
}

// Pide la orden y el repuesto, y llama a quitarRepuestoDeOrden
void menuQuitarRepuestoOrden(SistemaTaller* s) {
    cout << "\n--- QUITANDO REPUESTO DE ORDEN ---\n";
    int idOrden = leerEntero("Numero de orden: ");
    int idRepuesto = leerEntero("ID del repuesto a quitar: ");

    ResultadoOrden res = quitarRepuestoDeOrden(s, idOrden, idRepuesto);

    if (res.exito) {
        cout << "Repuesto quitado correctamente de la orden #" << idOrden << ".\n";
    } else {
        cout << "No se pudo quitar el repuesto (verifique el estado de la orden o si existe el repuesto).\n";
    }
}

// Pide la orden y la mano de obra, pide confirmacion y llama a cerrarOrden
void menuCerrarOrden(SistemaTaller* s) {
    cout << "\n--- CERRAR ORDEN DE SERVICIO ---\n";
    int idOrden = leerEntero("Numero de orden a cerrar: ");
    double manoDeObra = leerDouble("Costo de mano de obra: ");

    char conf = leerChar("¿Desea confirmar el cierre de la orden #" << idOrden << "? (S/N): ");
    if (conf != 'S' && conf != 's') {
        cout << "Operacion cancelada.\n";
        return;
    }

    ResultadoOrden res = cerrarOrden(s, idOrden, manoDeObra);

    if (res.exito) {
        cout << "Orden #" << idOrden << " cerrada exitosamente.\n";
    } else {
        if (res.codigoError == ERROR_ORDEN_VACIA) {
            cout << "Error: La orden #" << idOrden << " no tiene repuestos y la mano de obra es 0.\n";
        } else {
            cout << "Error al cerrar la orden (verifique el estado actual).\n";
        }
    }
}

// Pide la orden, pide confirmacion y llama a anularOrden
void menuAnularOrden(SistemaTaller* s) {
    cout << "\n--- ANULAR ORDEN DE SERVICIO ---\n";
    int idOrden = leerEntero("Numero de orden a anular: ");

    char conf = leerChar("¿Esta seguro de que desea anular la orden #" << idOrden << "? (S/N): ");
    if (conf != 'S' && conf != 's') {
        cout << "Operacion cancelada.\n";
        return;
    }

    ResultadoOrden res = anularOrden(s, idOrden);

    if (res.exito) {
        cout << "Orden #" << idOrden << " anulada correctamente.\n";
    } else {
        cout << "No se pudo anular la orden (puede que ya este cerrada o anulada).\n";
    }
}

// Pide el ID y muestra el detalle completo con mostrarDetalleOrden
void menuVerDetalleOrden(SistemaTaller* s) {
    cout << "\n--- VER DETALLE DE ORDEN ---\n";
    int idOrden = leerEntero("Ingrese el ID de la orden: ");

    OrdenServicio* orden = buscarOrdenPorId(s, idOrden); // Asumiendo que esta funcion existe en logica/ordenes.h
    if (orden != nullptr) {
        mostrarDetalleOrden(s, orden);
    } else {
        cout << "No se encontro la orden con ID #" << idOrden << ".\n";
    }
}

// Lista todas, por estado o por vehiculo (historial)
// Al final, delete[] del arreglo que devuelva la funcion de logica
void menuListarOrdenes(SistemaTaller* s) {
    cout << "\n--- LISTAR ORDENES ---\n"
         << "1. Todas las ordenes\n"
         << "2. Por estado\n"
         << "3. Historial por vehiculo (Placa)\n"
         << "Seleccione una opcion: ";
    int opt = leerEntero();

    OrdenServicio** ordenes = nullptr;
    int cantidad = 0;

    if (opt == 1) {
        ordenes = listarTodasLasOrdenes(s, &cantidad);
    } else if (opt == 2) {
        int estado = leerEntero("Ingrese el estado (ej. 0: Abierta, 1: Cerrada, etc.): ");
        ordenes = listarOrdenesPorEstado(s, estado, &cantidad);
    } else if (opt == 3) {
        string placa = leerCadena("Ingrese la placa del vehiculo: ");
        ordenes = listarOrdenesPorVehiculo(s, placa.c_str(), &cantidad);
    } else {
        cout << "Opcion invalida.\n";
        return;
    }

    mostrarListaOrdenes(s, ordenes, cantidad);

    // Liberar memoria del arreglo dinamico devuelto
    if (ordenes != nullptr) {
        delete[] ordenes;
    }
}

// Muestra la orden con el formato de la seccion 9.1 del PDF:
// estado, fechas, vehiculo, dueno, mecanico, falla, lineas de repuestos,
// subtotal, mano de obra y total
void mostrarDetalleOrden(SistemaTaller* s, OrdenServicio* orden) {
    if (orden == nullptr) return;

    cout << "\n========================================\n"
         << "        DETALLE DE ORDEN #" << orden->id << "\n"
         << "========================================\n"
         << "Estado: " << obtenerTextoEstado(orden->estado) << "\n"
         << "Fecha apertura: " << orden->fechaApertura << "\n"
         << "Fecha cierre:   " << orden->fechaCierre << "\n"
         << "Vehiculo (Placa): " << orden->placaVehiculo << "\n"
         << "Dueno: " << orden->nombreDueno << "\n"
         << "Mecanico ID: " << orden->idMecanico << "\n"
         << "Falla reportada: " << orden->falla << "\n"
         << "----------------------------------------\n"
         << "Lineas de repuestos:\n";

    for (int i = 0; i < orden->cantidadRepuestos; i++) {
        cout << " - " << orden->repuestos[i].nombreRepuesto
             << " | Cant: " << orden->repuestos[i].cantidad
             << " | Subtotal: $" << fixed << setprecision(2) << orden->repuestos[i].subtotal << "\n";
    }

    double subtotalRep = calcularSubtotalRepuestos(orden);
    double totalOrden = calcularTotalOrden(orden);

    cout << "----------------------------------------\n"
         << "Subtotal repuestos: $" << fixed << setprecision(2) << subtotalRep << "\n"
         << "Mano de obra:       $" << fixed << setprecision(2) << orden->manoDeObra << "\n"
         << "TOTAL:              $" << fixed << setprecision(2) << totalOrden << "\n"
         << "========================================\n";
}

// Muestra una lista corta de ordenes: numero, placa, estado y total
void mostrarListaOrdenes(SistemaTaller* s, OrdenServicio** ordenes, int cantidad) {
    if (ordenes == nullptr || cantidad <= 0) {
        cout << "\nNo hay ordenes para mostrar.\n";
        return;
    }

    cout << "\n" << left << setw(8) << "NUMERO" 
         << setw(12) << "PLACA" 
         << setw(15) << "ESTADO" 
         << setw(10) << "TOTAL" << "\n";
    cout << string(45, '-') << "\n";

    for (int i = 0; i < cantidad; i++) {
        if (ordenes[i] != nullptr) {
