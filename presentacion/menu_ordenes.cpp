#include "menu_ordenes.h"
#include "entrada.h"
#include "../logica/ordenes.h"
#include "../logica/vehiculos.h"
#include "../logica/mecanicos.h"
#include "../logica/repuestos.h"
#include "../logica/utilidades.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// Muestra el submenu de ordenes de servicio
void menuOrdenes(SistemaTaller* s) {
    int opcion = -1;
    do {
        cout << "\n--- MENU ORDENES DE SERVICIO ---\n"
             << "1. Abrir orden de servicio\n"
             << "2. Agregar repuesto a orden\n"
             << "3. Quitar repuesto de orden\n"
             << "4. Cerrar orden de servicio\n"
             << "5. Anular orden de servicio\n"
             << "6. Ver detalle de orden\n"
             << "7. Listar ordenes\n"
             << "0. Volver al menu principal\n";
        opcion = leerOpcion("Seleccione una opcion", 0, 7);

        switch (opcion) {
            case 1: menuAbrirOrden(s); break;
            case 2: menuAgregarRepuestoOrden(s); break;
            case 3: menuQuitarRepuestoOrden(s); break;
            case 4: menuCerrarOrden(s); break;
            case 5: menuAnularOrden(s); break;
            case 6: menuVerDetalleOrden(s); break;
            case 7: menuListarOrdenes(s); break;
            case 0: cout << "Regresando...\n"; break;
        }
    } while (opcion != 0 && opcion != -1); // -1: se acabo la entrada
}

// Pide placa del vehiculo, ID del mecanico, kilometraje y falla,
// pide confirmacion y llama a abrirOrden.
void menuAbrirOrden(SistemaTaller* s) {
    cout << "\n--- ABRIR ORDEN DE SERVICIO (Escriba CANCELAR para abortar) ---\n";
    char placa[10];
    int idMecanico, kilometraje;
    char falla[200];

    if (!leerTexto("Placa del vehiculo: ", placa, 10)) return;
    convertirAMayusculas(placa);
    Vehiculo* vehiculo = buscarVehiculoPorPlaca(s, placa);
    if (vehiculo == nullptr) {
        cout << "Error: No existe un vehiculo con la placa " << placa << ".\n";
        return;
    }

    if (!leerEntero("ID del mecanico: ", &idMecanico)) return;
    if (!leerEntero("Kilometraje actual: ", &kilometraje)) return;
    if (!leerTexto("Descripcion de la falla: ", falla, 200)) return;

    if (!confirmar("Confirma la apertura de la orden? (S/N): ")) {
        cout << "Operacion cancelada.\n";
        return;
    }

    int idNuevaOrden = 0;
    Resultado res = abrirOrden(s, vehiculo->id, idMecanico, kilometraje, falla, &idNuevaOrden);

    if (res == OK) {
        cout << "Orden #" << idNuevaOrden << " abierta exitosamente.\n";
    } else {
        switch (res) {
            case ERROR_NO_EXISTE:
                cout << "Error: No existe un mecanico con ese ID.\n";
                break;
            case ERROR_VEHICULO_CON_ORDEN_ABIERTA:
                cout << "Error: El vehiculo " << placa << " ya tiene una orden abierta.\n";
                break;
            case ERROR_MECANICO_SIN_CUPO:
                cout << "Error: El mecanico seleccionado ya tiene 3 ordenes abiertas.\n";
                break;
            case ERROR_KILOMETRAJE_MENOR:
                cout << "Error: El kilometraje es menor al ultimo registrado (" << vehiculo->kilometraje << " km).\n";
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
    int idOrden, idRepuesto, cantidad;
    if (!leerEntero("Numero de orden: ", &idOrden)) return;
    if (!leerEntero("ID del repuesto: ", &idRepuesto)) return;
    if (!leerEntero("Cantidad: ", &cantidad)) return;

    Resultado res = agregarRepuestoAOrden(s, idOrden, idRepuesto, cantidad);

    if (res == OK) {
        cout << "Repuesto agregado correctamente a la orden #" << idOrden << ".\n";
    } else {
        switch (res) {
            case ERROR_NO_EXISTE:
                cout << "Error: No existe esa orden o ese repuesto.\n";
                break;
            case ERROR_DATO_INVALIDO:
                cout << "Error: La cantidad debe ser mayor a 0.\n";
                break;
            case ERROR_STOCK_INSUFICIENTE: {
                Repuesto* repuesto = buscarRepuestoPorID(s, idRepuesto);
                cout << "Error: Stock insuficiente para " << repuesto->nombre
                     << " (disponible: " << repuesto->stock << ").\n";
                break;
            }
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
    int idOrden, idRepuesto;
    if (!leerEntero("Numero de orden: ", &idOrden)) return;
    if (!leerEntero("ID del repuesto a quitar: ", &idRepuesto)) return;

    Resultado res = quitarRepuestoDeOrden(s, idOrden, idRepuesto);

    if (res == OK) {
        cout << "Repuesto quitado correctamente de la orden #" << idOrden << ".\n";
    } else if (res == ERROR_ESTADO_INVALIDO) {
        cout << "Error: La orden #" << idOrden << " no esta ABIERTA.\n";
    } else {
        cout << "No se pudo quitar el repuesto (verifique que la orden exista y tenga ese repuesto).\n";
    }
}

// Pide la orden y la mano de obra, pide confirmacion y llama a cerrarOrden
void menuCerrarOrden(SistemaTaller* s) {
    cout << "\n--- CERRAR ORDEN DE SERVICIO ---\n";
    int idOrden;
    float manoDeObra;
    if (!leerEntero("Numero de orden a cerrar: ", &idOrden)) return;
    if (!leerFlotante("Costo de mano de obra: ", &manoDeObra)) return;

    string pregunta = "Desea confirmar el cierre de la orden #" + to_string(idOrden) + "? (S/N): ";
    if (!confirmar(pregunta.c_str())) {
        cout << "Operacion cancelada.\n";
        return;
    }

    Resultado res = cerrarOrden(s, idOrden, manoDeObra);

    if (res == OK) {
        cout << "Orden #" << idOrden << " cerrada exitosamente.\n";
    } else if (res == ERROR_ORDEN_VACIA) {
        cout << "Error: La orden #" << idOrden << " no tiene repuestos y la mano de obra es 0.\n";
    } else if (res == ERROR_NO_EXISTE) {
        cout << "Error: No existe la orden #" << idOrden << ".\n";
    } else if (res == ERROR_DATO_INVALIDO) {
        cout << "Error: La mano de obra no puede ser negativa.\n";
    } else {
        cout << "Error al cerrar la orden (solo se pueden cerrar ordenes ABIERTAS).\n";
    }
}

// Pide la orden, pide confirmacion y llama a anularOrden
void menuAnularOrden(SistemaTaller* s) {
    cout << "\n--- ANULAR ORDEN DE SERVICIO ---\n";
    int idOrden;
    if (!leerEntero("Numero de orden a anular: ", &idOrden)) return;

    string pregunta = "Esta seguro de que desea anular la orden #" + to_string(idOrden) + "? (S/N): ";
    if (!confirmar(pregunta.c_str())) {
        cout << "Operacion cancelada.\n";
        return;
    }

    Resultado res = anularOrden(s, idOrden);

    if (res == OK) {
        cout << "Orden #" << idOrden << " anulada correctamente. Los repuestos volvieron al inventario.\n";
    } else if (res == ERROR_NO_EXISTE) {
        cout << "Error: No existe la orden #" << idOrden << ".\n";
    } else {
        cout << "No se pudo anular la orden (ya esta anulada).\n";
    }
}

// Pide el ID y muestra el detalle completo con mostrarDetalleOrden
void menuVerDetalleOrden(SistemaTaller* s) {
    cout << "\n--- VER DETALLE DE ORDEN ---\n";
    int idOrden;
    if (!leerEntero("Ingrese el ID de la orden: ", &idOrden)) return;

    OrdenServicio* orden = buscarOrdenPorID(s, idOrden);
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
         << "3. Historial por vehiculo (Placa)\n";
    int opt = leerOpcion("Seleccione una opcion", 1, 3);

    OrdenServicio** ordenes = nullptr;
    int cantidad = 0;

    if (opt == 1) {
        ordenes = listarOrdenes(s, &cantidad);
    } else if (opt == 2) {
        cout << "1. ABIERTA\n2. CERRADA\n3. ANULADA\n";
        int e = leerOpcion("Estado", 1, 3);
        if (e == 0) return;
        const char* estados[] = {"ABIERTA", "CERRADA", "ANULADA"};
        ordenes = listarOrdenesPorEstado(s, estados[e - 1], &cantidad);
    } else if (opt == 3) {
        char placa[10];
        if (!leerTexto("Ingrese la placa del vehiculo: ", placa, 10)) return;
        convertirAMayusculas(placa);
        Vehiculo* vehiculo = buscarVehiculoPorPlaca(s, placa);
        if (vehiculo == nullptr) {
            cout << "No existe un vehiculo con la placa " << placa << ".\n";
            return;
        }
        ordenes = listarOrdenesPorVehiculo(s, vehiculo->id, &cantidad);
    } else {
        return;
    }

    mostrarListaOrdenes(s, ordenes, cantidad);

    // Liberar memoria del arreglo dinamico devuelto
    delete[] ordenes;
}

// Muestra la orden con el formato de la seccion 9.1 del PDF:
// estado, fechas, vehiculo, dueno, mecanico, falla, lineas de repuestos,
// subtotal, mano de obra y total
void mostrarDetalleOrden(SistemaTaller* s, OrdenServicio* orden) {
    if (orden == nullptr) return;

    Vehiculo* vehiculo = buscarVehiculoPorID(s, orden->idVehiculo);
    Mecanico* mecanico = buscarMecanicoPorID(s, orden->idMecanico);

    cout << fixed << setprecision(2);
    cout << "\n+----------------------------------------------------------------+\n"
         << "  ORDEN DE SERVICIO #" << orden->id << "\n"
         << "+----------------------------------------------------------------+\n"
         << "  Estado   : " << left << setw(18) << orden->estado << "Apertura: " << orden->fechaApertura << "\n";
    if (orden->fechaCierre[0] != '\0') {
        cout << "  " << setw(29) << " " << "Cierre  : " << orden->fechaCierre << "\n";
    }
    if (vehiculo != nullptr) {
        cout << "  Vehiculo : " << vehiculo->placa << " - " << vehiculo->marca << " " << vehiculo->modelo
             << " " << vehiculo->anio << " (" << orden->kilometrajeIngreso << " km)\n"
             << "  Dueno    : " << vehiculo->nombrePropietario << "\n";
    }
    if (mecanico != nullptr) {
        cout << "  Mecanico : " << mecanico->nombre << " (" << mecanico->especialidad << ")\n";
    }
    cout << "  Falla    : " << orden->descripcionFalla << "\n"
         << "+----------+---------------------------+------+---------+--------+\n"
         << "  " << left << setw(10) << "Codigo" << setw(28) << "Repuesto"
         << right << setw(5) << "Cant" << setw(10) << "P.Unit" << setw(9) << "Total" << "\n"
         << "+----------+---------------------------+------+---------+--------+\n";

    for (int i = 0; i < orden->numDetalles; i++) {
        DetalleOrden* linea = &orden->detalles[i];
        Repuesto* repuesto = buscarRepuestoPorID(s, linea->idRepuesto);
        cout << "  " << left << setw(10) << (repuesto ? repuesto->codigo : "?")
             << setw(28) << (repuesto ? repuesto->nombre : "(repuesto eliminado)")
             << right << setw(5) << linea->cantidad
             << setw(10) << linea->precioUnitario
             << setw(9) << linea->cantidad * linea->precioUnitario << "\n";
    }

    cout << "+----------+---------------------------+------+---------+--------+\n"
         << right << setw(55) << "Repuestos" << setw(9) << calcularSubtotalRepuestos(orden) << "\n"
         << setw(55) << "Mano de obra" << setw(9) << orden->manoObra << "\n"
         << setw(55) << "TOTAL" << setw(9) << calcularTotalOrden(orden) << "\n"
         << "+----------------------------------------------------------------+\n";
    cout << left; // se deja la alineacion como estaba para el resto del programa
}

// Muestra una lista corta de ordenes: numero, placa, estado y total
void mostrarListaOrdenes(SistemaTaller* s, OrdenServicio** ordenes, int cantidad) {
    if (ordenes == nullptr || cantidad <= 0) {
        cout << "\nNo hay ordenes para mostrar.\n";
        return;
    }

    cout << "\n" << left << setw(8) << "NUMERO"
         << setw(12) << "PLACA"
         << setw(12) << "ESTADO"
         << setw(12) << "APERTURA"
         << right << setw(10) << "TOTAL" << "\n";
    cout << string(54, '-') << "\n";

    cout << fixed << setprecision(2);
    for (int i = 0; i < cantidad; i++) {
        if (ordenes[i] != nullptr) {
            Vehiculo* vehiculo = buscarVehiculoPorID(s, ordenes[i]->idVehiculo);
            cout << left << setw(8) << ordenes[i]->id
                 << setw(12) << (vehiculo ? vehiculo->placa : "?")
                 << setw(12) << ordenes[i]->estado
                 << setw(12) << ordenes[i]->fechaApertura
                 << right << setw(10) << calcularTotalOrden(ordenes[i]) << "\n";
        }
    }
    cout << left << "Total de ordenes: " << cantidad << "\n";
}
