#include "sistema.h"

void inicializarSistema(SistemaTaller* s, Taller datosTaller) {
    // el taller se carga una sola vez, no tiene CRUD
    s->taller = datosTaller;

    s->vehiculos = new Vehiculo[CAPACIDAD_INICIAL_ENTIDADES];
    s->numVehiculos = 0;
    s->capacidadVehiculos = CAPACIDAD_INICIAL_ENTIDADES;

    s->mecanicos = new Mecanico[CAPACIDAD_INICIAL_ENTIDADES];
    s->numMecanicos = 0;
    s->capacidadMecanicos = CAPACIDAD_INICIAL_ENTIDADES;

    s->repuestos = new Repuesto[CAPACIDAD_INICIAL_ENTIDADES];
    s->numRepuestos = 0;
    s->capacidadRepuestos = CAPACIDAD_INICIAL_ENTIDADES;

    s->ordenes = new OrdenServicio[CAPACIDAD_INICIAL_ENTIDADES];
    s->numOrdenes = 0;
    s->capacidadOrdenes = CAPACIDAD_INICIAL_ENTIDADES;

    // los IDs arrancan en 1, el 0 lo dejamos como "no existe"
    s->siguienteIdVehiculo = 1;
    s->siguienteIdMecanico = 1;
    s->siguienteIdRepuesto = 1;
    s->siguienteIdOrden = 1;
}

void liberarSistema(SistemaTaller* s) {
    // primero los detalles de cada orden: si borramos las ordenes antes,
    // perdemos los punteros y esa memoria queda huerfana
    for (int i = 0; i < s->numOrdenes; i++) {
        delete[] s->ordenes[i].detalles;
        s->ordenes[i].detalles = nullptr;
    }

    delete[] s->vehiculos;
    delete[] s->mecanicos;
    delete[] s->repuestos;
    delete[] s->ordenes;

    s->vehiculos = nullptr;
    s->mecanicos = nullptr;
    s->repuestos = nullptr;
    s->ordenes = nullptr;

    s->numVehiculos = s->numMecanicos = s->numRepuestos = s->numOrdenes = 0;
    s->capacidadVehiculos = s->capacidadMecanicos = 0;
    s->capacidadRepuestos = s->capacidadOrdenes = 0;
}

void asegurarCapacidadVehiculos(SistemaTaller* s) {
    if (s->numVehiculos < s->capacidadVehiculos) return; // todavia hay espacio, no hace falta crecer

    int nuevaCapacidad = s->capacidadVehiculos * 2;
    Vehiculo* nuevoArreglo = new Vehiculo[nuevaCapacidad];

    for (int i = 0; i < s->numVehiculos; i++) {
        nuevoArreglo[i] = s->vehiculos[i];
    }

    delete[] s->vehiculos;
    s->vehiculos = nuevoArreglo;
    s->capacidadVehiculos = nuevaCapacidad;
    // Ojo: cualquier Vehiculo* guardado de antes ahora apunta a memoria liberada
}

void asegurarCapacidadMecanicos(SistemaTaller* s) {
    if (s->numMecanicos < s->capacidadMecanicos) return;

    int nuevaCapacidad = s->capacidadMecanicos * 2;
    Mecanico* nuevoArreglo = new Mecanico[nuevaCapacidad];

    for (int i = 0; i < s->numMecanicos; i++) {
        nuevoArreglo[i] = s->mecanicos[i];
    }

    delete[] s->mecanicos;
    s->mecanicos = nuevoArreglo;
    s->capacidadMecanicos = nuevaCapacidad;
}

void asegurarCapacidadRepuestos(SistemaTaller* s) {
    if (s->numRepuestos < s->capacidadRepuestos) return;

    int nuevaCapacidad = s->capacidadRepuestos * 2;
    Repuesto* nuevoArreglo = new Repuesto[nuevaCapacidad];

    for (int i = 0; i < s->numRepuestos; i++) {
        nuevoArreglo[i] = s->repuestos[i];
    }

    delete[] s->repuestos;
    s->repuestos = nuevoArreglo;
    s->capacidadRepuestos = nuevaCapacidad;
}

void asegurarCapacidadOrdenes(SistemaTaller* s) {
    if (s->numOrdenes < s->capacidadOrdenes) return;

    int nuevaCapacidad = s->capacidadOrdenes * 2;
    OrdenServicio* nuevoArreglo = new OrdenServicio[nuevaCapacidad];

    for (int i = 0; i < s->numOrdenes; i++) {
        // se copia el struct completo, incluido el puntero a detalles
        // los detalles NO se duplican: la orden nueva se queda con el mismo
        // arreglo, por eso abajo NO hacemos delete[] de los detalles viejos
        nuevoArreglo[i] = s->ordenes[i];
    }

    delete[] s->ordenes; // solo las ordenes, los detalles siguen vivos
    s->ordenes = nuevoArreglo;
    s->capacidadOrdenes = nuevaCapacidad;
}

void asegurarCapacidadDetalles(OrdenServicio* orden) {
    if (orden->numDetalles < orden->capacidadDetalles) return;

    int nuevaCapacidad = orden->capacidadDetalles * 2;
    DetalleOrden* nuevasLineas = new DetalleOrden[nuevaCapacidad];

    for (int i = 0; i < orden->numDetalles; i++) {
        nuevasLineas[i] = orden->detalles[i];
    }

    delete[] orden->detalles;
    orden->detalles = nuevasLineas;
    orden->capacidadDetalles = nuevaCapacidad;
}
