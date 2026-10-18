#include "mecanicos.h"
#include "sistema.h"
#include "utilidades.h"
#include <cstring>

// Registra un mecanico nuevo despues de validar sus datos
Resultado registrarMecanico(SistemaTaller* s, Mecanico datos) {
    convertirAMayusculas(datos.especialidad);

    if (esTextoVacio(datos.nombre)) return ERROR_DATO_INVALIDO;
    if (!esCedulaValida(datos.cedula)) return ERROR_DATO_INVALIDO;
    if (!esEspecialidadValida(datos.especialidad)) return ERROR_DATO_INVALIDO;
    if (!esFechaValida(datos.fechaIngreso)) return ERROR_DATO_INVALIDO;
    if (buscarMecanicoPorCedula(s, datos.cedula) != nullptr) return ERROR_DUPLICADO;

    asegurarCapacidadMecanicos(s);

    datos.id = s->siguienteIdMecanico;
    s->siguienteIdMecanico++;

    s->mecanicos[s->numMecanicos] = datos;
    s->numMecanicos++;
    return OK;
}

// Busca un mecanico por su ID. Mismo truco que con los vehiculos
Mecanico* buscarMecanicoPorID(SistemaTaller* s, int id) {
    for (int i = 0; i < s->numMecanicos; i++) {
        if (s->mecanicos[i].id == id) {
            return &s->mecanicos[i];
        }
    }
    return nullptr;
}

// Busca un mecanico por su cedula exacta. Se usa para no repetir cedulas
Mecanico* buscarMecanicoPorCedula(SistemaTaller* s, const char* cedula) {
    for (int i = 0; i < s->numMecanicos; i++) {
        if (strcmp(s->mecanicos[i].cedula, cedula) == 0) {
            return &s->mecanicos[i];
        }
    }
    return nullptr;
}

Mecanico** listarMecanicos(SistemaTaller* s, int* cantidad) {
    Mecanico** resultado = new Mecanico*[s->numMecanicos];
    for (int i = 0; i < s->numMecanicos; i++) {
        resultado[i] = &s->mecanicos[i];
    }
    *cantidad = s->numMecanicos;
    return resultado;
}

// Recorre las ordenes y cuenta las ABIERTAS que tiene asignadas este mecanico
int contarOrdenesAbiertasMecanico(SistemaTaller* s, int idMecanico) {
    int abiertas = 0;
    for (int i = 0; i < s->numOrdenes; i++) {
        if (s->ordenes[i].idMecanico == idMecanico &&
            strcmp(s->ordenes[i].estado, "ABIERTA") == 0) {
            abiertas++;
        }
    }
    return abiertas;
}
