#include "repuestos.h"
#include "sistema.h"
#include "utilidades.h"
#include <cstring>

// Registra un repuesto nuevo despues de validar sus datos
Resultado registrarRepuesto(SistemaTaller* s, Repuesto datos) {
    if (esTextoVacio(datos.codigo)) return ERROR_DATO_INVALIDO;
    if (esTextoVacio(datos.nombre) || esTextoVacio(datos.marca)) return ERROR_DATO_INVALIDO;
    if (datos.precio <= 0) return ERROR_DATO_INVALIDO;
    if (datos.stock < 0 || datos.stockMinimo < 0) return ERROR_DATO_INVALIDO;
    if (buscarRepuestoPorCodigo(s, datos.codigo) != nullptr) return ERROR_DUPLICADO;

    asegurarCapacidadRepuestos(s);

    datos.id = s->siguienteIdRepuesto;
    s->siguienteIdRepuesto++;
    obtenerFechaActual(datos.fechaRegistro);

    s->repuestos[s->numRepuestos] = datos;
    s->numRepuestos++;
    return OK;
}

Repuesto* buscarRepuestoPorID(SistemaTaller* s, int id) {
    for (int i = 0; i < s->numRepuestos; i++) {
        if (s->repuestos[i].id == id) {
            return &s->repuestos[i];
        }
    }
    return nullptr;
}

// Busca un repuesto por su codigo exacto. Se usa para no repetir codigos
Repuesto* buscarRepuestoPorCodigo(SistemaTaller* s, const char* codigo) {
    for (int i = 0; i < s->numRepuestos; i++) {
        if (strcmp(s->repuestos[i].codigo, codigo) == 0) {
            return &s->repuestos[i];
        }
    }
    return nullptr;
}

// Devuelve los repuestos cuyo codigo o nombre contiene el texto buscado
Repuesto** buscarRepuestos(SistemaTaller* s, const char* texto, int* cantidad) {
    // primera pasada: contar coincidencias
    int encontrados = 0;
    for (int i = 0; i < s->numRepuestos; i++) {
        if (contieneSinMayusculas(s->repuestos[i].codigo, texto) ||
            contieneSinMayusculas(s->repuestos[i].nombre, texto)) {
            encontrados++;
        }
    }

    // segunda pasada: guardar las direcciones
    Repuesto** resultado = new Repuesto*[encontrados];
    int posicion = 0;
    for (int i = 0; i < s->numRepuestos; i++) {
        if (contieneSinMayusculas(s->repuestos[i].codigo, texto) ||
            contieneSinMayusculas(s->repuestos[i].nombre, texto)) {
            resultado[posicion] = &s->repuestos[i];
            posicion++;
        }
    }

    *cantidad = encontrados;
    return resultado;
}

Repuesto** listarRepuestos(SistemaTaller* s, int* cantidad) {
    Repuesto** resultado = new Repuesto*[s->numRepuestos];
    for (int i = 0; i < s->numRepuestos; i++) {
        resultado[i] = &s->repuestos[i];
    }
    *cantidad = s->numRepuestos;
    return resultado;
}

// Llego mercancia: se suman las unidades al stock del repuesto
Resultado registrarEntradaMercancia(SistemaTaller* s, int idRepuesto, int cantidad) {
    Repuesto* repuesto = buscarRepuestoPorID(s, idRepuesto);
    if (repuesto == nullptr) return ERROR_NO_EXISTE;
    if (cantidad <= 0) return ERROR_DATO_INVALIDO;

    repuesto->stock += cantidad;
    return OK;
}

// Cambia el precio vigente. Las ordenes anteriores no cambian porque
// cada linea de detalle guarda su propio precioUnitario
Resultado cambiarPrecioRepuesto(SistemaTaller* s, int idRepuesto, float nuevoPrecio) {
    Repuesto* repuesto = buscarRepuestoPorID(s, idRepuesto);
    if (repuesto == nullptr) return ERROR_NO_EXISTE;
    if (nuevoPrecio <= 0) return ERROR_DATO_INVALIDO;

    repuesto->precio = nuevoPrecio;
    return OK;
}

// Elimina un repuesto que nunca se uso en una orden. Los siguientes
// se corren una posicion hacia la izquierda
Resultado eliminarRepuesto(SistemaTaller* s, int idRepuesto) {
    int posicion = -1;
    for (int i = 0; i < s->numRepuestos; i++) {
        if (s->repuestos[i].id == idRepuesto) {
            posicion = i;
            break;
        }
    }
    if (posicion == -1) return ERROR_NO_EXISTE;
    if (repuestoUsadoEnOrdenes(s, idRepuesto)) return ERROR_EN_USO; // si ya se uso en una orden, no se puede borrar

    for (int i = posicion; i < s->numRepuestos - 1; i++) {
        s->repuestos[i] = s->repuestos[i + 1];
    }
    s->numRepuestos--;
    return OK;
}

// true si ya toca pedir mas: el stock llego al minimo o por debajo
bool tieneBajoStock(Repuesto* repuesto) {
    return repuesto->stock <= repuesto->stockMinimo;
}

// Revisa las lineas de detalle de todas las ordenes, sin importar su estado
bool repuestoUsadoEnOrdenes(SistemaTaller* s, int idRepuesto) {
    for (int i = 0; i < s->numOrdenes; i++) {
        OrdenServicio* orden = &s->ordenes[i];
        for (int j = 0; j < orden->numDetalles; j++) {
            if (orden->detalles[j].idRepuesto == idRepuesto) {
                return true;
            }
        }
    }
    return false;
}
