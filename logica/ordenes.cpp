#include "ordenes.h"
#include "sistema.h"
#include "utilidades.h"
#include "vehiculos.h"
#include "mecanicos.h"
#include "repuestos.h"
#include <cstring>

// Abre una orden de servicio. Revisa las reglas en este orden:
// vehiculo y mecanico existen, el vehiculo no tiene otra orden ABIERTA,
// el mecanico tiene menos de 3 ABIERTAS y el kilometraje no retrocede
Resultado abrirOrden(SistemaTaller* s, int idVehiculo, int idMecanico,
                     int kilometrajeIngreso, const char* descripcionFalla, int* idNuevaOrden) {
    Vehiculo* vehiculo = buscarVehiculoPorID(s, idVehiculo);
    if (vehiculo == nullptr) return ERROR_NO_EXISTE;
    if (buscarMecanicoPorID(s, idMecanico) == nullptr) return ERROR_NO_EXISTE;

    if (buscarOrdenAbiertaDeVehiculo(s, idVehiculo) != nullptr) return ERROR_VEHICULO_CON_ORDEN_ABIERTA;
    if (contarOrdenesAbiertasMecanico(s, idMecanico) >= 3) return ERROR_MECANICO_SIN_CUPO; // maximo 3 ordenes abiertas por mecanico
    if (kilometrajeIngreso < vehiculo->kilometraje) return ERROR_KILOMETRAJE_MENOR; // el kilometraje no puede bajar

    asegurarCapacidadOrdenes(s);

    OrdenServicio nueva;
    nueva.id = s->siguienteIdOrden;
    s->siguienteIdOrden++;
    nueva.idVehiculo = idVehiculo;
    nueva.idMecanico = idMecanico;
    obtenerFechaActual(nueva.fechaApertura);
    nueva.fechaCierre[0] = '\0'; // vacia mientras este ABIERTA
    copiarTexto(nueva.estado, "ABIERTA", sizeof(nueva.estado));
    copiarTexto(nueva.descripcionFalla, descripcionFalla, sizeof(nueva.descripcionFalla));
    nueva.kilometrajeIngreso = kilometrajeIngreso;
    nueva.manoObra = 0;

    // cada orden tiene su propio arreglo de detalles, que empieza con capacidad 2
    nueva.detalles = new DetalleOrden[CAPACIDAD_INICIAL_DETALLES];
    nueva.numDetalles = 0;
    nueva.capacidadDetalles = CAPACIDAD_INICIAL_DETALLES;

    s->ordenes[s->numOrdenes] = nueva;
    s->numOrdenes++;

    // el vehiculo queda con el kilometraje con el que ingreso al taller
    vehiculo->kilometraje = kilometrajeIngreso;

    *idNuevaOrden = nueva.id;
    return OK;
}

// Agrega un repuesto a una orden ABIERTA y descuenta las unidades del stock
Resultado agregarRepuestoAOrden(SistemaTaller* s, int idOrden, int idRepuesto, int cantidad) {
    OrdenServicio* orden = buscarOrdenPorID(s, idOrden);
    if (orden == nullptr) return ERROR_NO_EXISTE;
    if (strcmp(orden->estado, "ABIERTA") != 0) return ERROR_ESTADO_INVALIDO;

    Repuesto* repuesto = buscarRepuestoPorID(s, idRepuesto);
    if (repuesto == nullptr) return ERROR_NO_EXISTE;
    if (cantidad <= 0) return ERROR_DATO_INVALIDO;
    if (repuesto->stock < cantidad) return ERROR_STOCK_INSUFICIENTE;

    DetalleOrden* lineaExistente = buscarDetalleEnOrden(orden, idRepuesto);
    if (lineaExistente != nullptr) {
        // el repuesto ya estaba en la orden: se acumula en la misma linea
        lineaExistente->cantidad += cantidad;
    } else {
        // linea nueva, con el precio que tiene el repuesto en este momento
        asegurarCapacidadDetalles(orden);
        DetalleOrden linea;
        linea.idRepuesto = idRepuesto;
        linea.cantidad = cantidad;
        linea.precioUnitario = repuesto->precio;
        orden->detalles[orden->numDetalles] = linea;
        orden->numDetalles++;
    }

    repuesto->stock -= cantidad;
    return OK;
}

// Quita la linea completa de un repuesto y devuelve todas sus unidades al stock
Resultado quitarRepuestoDeOrden(SistemaTaller* s, int idOrden, int idRepuesto) {
    OrdenServicio* orden = buscarOrdenPorID(s, idOrden);
    if (orden == nullptr) return ERROR_NO_EXISTE;
    if (strcmp(orden->estado, "ABIERTA") != 0) return ERROR_ESTADO_INVALIDO;

    int posicion = -1;
    for (int i = 0; i < orden->numDetalles; i++) {
        if (orden->detalles[i].idRepuesto == idRepuesto) {
            posicion = i;
            break;
        }
    }
    if (posicion == -1) return ERROR_NO_EXISTE;

    // las unidades de la linea vuelven al inventario
    Repuesto* repuesto = buscarRepuestoPorID(s, idRepuesto);
    if (repuesto != nullptr) {
        repuesto->stock += orden->detalles[posicion].cantidad;
    }

    // las lineas siguientes se corren una posicion hacia la izquierda
    for (int i = posicion; i < orden->numDetalles - 1; i++) {
        orden->detalles[i] = orden->detalles[i + 1];
    }
    orden->numDetalles--;
    return OK;
}

// Cierra una orden ABIERTA. Debe tener al menos un repuesto o mano de obra mayor que 0
Resultado cerrarOrden(SistemaTaller* s, int idOrden, float manoObra) {
    OrdenServicio* orden = buscarOrdenPorID(s, idOrden);
    if (orden == nullptr) return ERROR_NO_EXISTE;
    if (strcmp(orden->estado, "ABIERTA") != 0) return ERROR_ESTADO_INVALIDO;
    if (manoObra < 0) return ERROR_DATO_INVALIDO;
    if (orden->numDetalles == 0 && manoObra == 0) return ERROR_ORDEN_VACIA; // no se cierra una orden vacia

    orden->manoObra = manoObra;
    copiarTexto(orden->estado, "CERRADA", sizeof(orden->estado));
    obtenerFechaActual(orden->fechaCierre);
    return OK;
}

// Anula una orden ABIERTA o CERRADA. Las unidades de todas sus lineas
// vuelven al stock. La orden no se borra: queda guardada como ANULADA
Resultado anularOrden(SistemaTaller* s, int idOrden) {
    OrdenServicio* orden = buscarOrdenPorID(s, idOrden);
    if (orden == nullptr) return ERROR_NO_EXISTE;
    if (strcmp(orden->estado, "ANULADA") == 0) return ERROR_ESTADO_INVALIDO;

    for (int i = 0; i < orden->numDetalles; i++) {
        Repuesto* repuesto = buscarRepuestoPorID(s, orden->detalles[i].idRepuesto);
        if (repuesto != nullptr) {
            repuesto->stock += orden->detalles[i].cantidad;
        }
    }

    copiarTexto(orden->estado, "ANULADA", sizeof(orden->estado));
    return OK;
}

// Suma lo que cuestan los repuestos de la orden: cantidad * precio de cada linea.
// Se usa el precio guardado en la linea, no el actual, asi un cambio de precio
// no le altera la cuenta a nadie que ya pago
float calcularSubtotalRepuestos(OrdenServicio* orden) {
    float subtotal = 0;
    for (int i = 0; i < orden->numDetalles; i++) {
        subtotal += orden->detalles[i].cantidad * orden->detalles[i].precioUnitario;
    }
    return subtotal;
}

float calcularTotalOrden(OrdenServicio* orden) {
    return calcularSubtotalRepuestos(orden) + orden->manoObra;
}

OrdenServicio* buscarOrdenPorID(SistemaTaller* s, int id) {
    for (int i = 0; i < s->numOrdenes; i++) {
        if (s->ordenes[i].id == id) {
            return &s->ordenes[i];
        }
    }
    return nullptr;
}

// Busca la orden ABIERTA de un vehiculo. Un vehiculo solo puede tener una
OrdenServicio* buscarOrdenAbiertaDeVehiculo(SistemaTaller* s, int idVehiculo) {
    for (int i = 0; i < s->numOrdenes; i++) {
        if (s->ordenes[i].idVehiculo == idVehiculo &&
            strcmp(s->ordenes[i].estado, "ABIERTA") == 0) {
            return &s->ordenes[i];
        }
    }
    return nullptr;
}

// Busca la linea de un repuesto dentro de una orden
DetalleOrden* buscarDetalleEnOrden(OrdenServicio* orden, int idRepuesto) {
    for (int i = 0; i < orden->numDetalles; i++) {
        if (orden->detalles[i].idRepuesto == idRepuesto) {
            return &orden->detalles[i];
        }
    }
    return nullptr;
}

// Cuenta las ordenes de un vehiculo, en cualquier estado
int contarOrdenesDeVehiculo(SistemaTaller* s, int idVehiculo) {
    int total = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (s->ordenes[i].idVehiculo == idVehiculo) {
            total++;
        }
    }
    return total;
}

OrdenServicio** listarOrdenes(SistemaTaller* s, int* cantidad) {
    OrdenServicio** resultado = new OrdenServicio*[s->numOrdenes];
    for (int i = 0; i < s->numOrdenes; i++) {
        resultado[i] = &s->ordenes[i];
    }
    *cantidad = s->numOrdenes;
    return resultado;
}

// Devuelve las ordenes que estan en un estado ("ABIERTA", "CERRADA" o "ANULADA")
OrdenServicio** listarOrdenesPorEstado(SistemaTaller* s, const char* estado, int* cantidad) {
    int encontradas = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (strcmp(s->ordenes[i].estado, estado) == 0) encontradas++;
    }

    OrdenServicio** resultado = new OrdenServicio*[encontradas];
    int posicion = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (strcmp(s->ordenes[i].estado, estado) == 0) {
            resultado[posicion] = &s->ordenes[i];
            posicion++;
        }
    }

    *cantidad = encontradas;
    return resultado;
}

// Devuelve el historial de ordenes de un vehiculo
OrdenServicio** listarOrdenesPorVehiculo(SistemaTaller* s, int idVehiculo, int* cantidad) {
    int encontradas = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (s->ordenes[i].idVehiculo == idVehiculo) encontradas++;
    }

    OrdenServicio** resultado = new OrdenServicio*[encontradas];
    int posicion = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (s->ordenes[i].idVehiculo == idVehiculo) {
            resultado[posicion] = &s->ordenes[i];
            posicion++;
        }
    }

    *cantidad = encontradas;
    return resultado;
}
