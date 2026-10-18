#include "vehiculos.h"
#include "sistema.h"
#include "utilidades.h"
#include "ordenes.h"
#include <cstring>

// Registra un vehiculo nuevo. Primero valida todos los datos y, solo si
// todo esta bien, lo guarda al final del arreglo
Vehiculo* agregarVehiculo(SistemaTaller* s, Vehiculo datos) {
    // la placa se guarda en mayusculas para que "ab123cd" y "AB123CD" sean la misma
    convertirAMayusculas(datos.placa);

    if (!esPlacaValida(datos.placa)) return nullptr;
    if (buscarVehiculoPorPlaca(s, datos.placa) != nullptr) return nullptr; // la placa ya existe
    if (datos.anio < 1950 || datos.anio > 2027) return nullptr; // el anio tiene que estar en el rango del enunciado
    if (datos.kilometraje < 0) return nullptr; // no hay kilometraje negativo
    if (esTextoVacio(datos.marca) || esTextoVacio(datos.modelo)) return nullptr;
    if (esTextoVacio(datos.nombrePropietario)) return nullptr;
    if (!esCedulaValida(datos.cedulaPropietario)) return nullptr;

    // si el arreglo esta lleno, se duplica su capacidad
    asegurarCapacidadVehiculos(s);

    // el sistema asigna el ID y la fecha de registro
    datos.id = s->siguienteIdVehiculo;
    s->siguienteIdVehiculo++;
    obtenerFechaActual(datos.fechaRegistro);

    // el vehiculo nuevo ocupa la primera posicion libre
    s->vehiculos[s->numVehiculos] = datos;
    s->numVehiculos++;

    return &s->vehiculos[s->numVehiculos - 1];
}

// Busca un vehiculo por su ID. Devuelve su direccion o nullptr si no existe
Vehiculo* buscarVehiculoPorID(SistemaTaller* s, int id) {
    for (int i = 0; i < s->numVehiculos; i++) {
        if (s->vehiculos[i].id == id) {
            return &s->vehiculos[i];
        }
    }
    return nullptr;
}

// Busca un vehiculo por su placa exacta
// Devuelve su direccion o nullptr si no existe
Vehiculo* buscarVehiculoPorPlaca(SistemaTaller* s, const char* placa) {
    for (int i = 0; i < s->numVehiculos; i++) {
        if (strcmp(s->vehiculos[i].placa, placa) == 0) {
            return &s->vehiculos[i];
        }
    }
    return nullptr;
}

// Devuelve un arreglo nuevo con las direcciones de los vehiculos cuyo
// propietario contiene el texto buscado, sin importar mayusculas
// Quien llama a esta funcion debe liberar el arreglo con delete[]
Vehiculo** buscarVehiculosPorPropietario(SistemaTaller* s, const char* subcadena, int* cantidad) {
    // primera pasada: contar cuantos coinciden, para saber de que tamano crear el arreglo
    int encontrados = 0;
    for (int i = 0; i < s->numVehiculos; i++) {
        if (contieneSinMayusculas(s->vehiculos[i].nombrePropietario, subcadena)) {
            encontrados++;
        }
    }

    // segunda pasada: guardar la direccion de cada vehiculo que coincide
    Vehiculo** resultado = new Vehiculo*[encontrados];
    int posicion = 0;
    for (int i = 0; i < s->numVehiculos; i++) {
        if (contieneSinMayusculas(s->vehiculos[i].nombrePropietario, subcadena)) {
            resultado[posicion] = &s->vehiculos[i];
            posicion++;
        }
    }

    *cantidad = encontrados;
    return resultado;
}

// Devuelve un arreglo nuevo con las direcciones de todos los vehiculos
// Quien llama a esta funcion debe liberar el arreglo con delete[]
Vehiculo** listarVehiculos(SistemaTaller* s, int* cantidad) {
    Vehiculo** resultado = new Vehiculo*[s->numVehiculos];
    for (int i = 0; i < s->numVehiculos; i++) {
        resultado[i] = &s->vehiculos[i];
    }
    *cantidad = s->numVehiculos;
    return resultado;
}

// Cambia marca, modelo, anio y datos del propietario
// El ID, la placa, el kilometraje y la fecha de registro no se modifican
bool actualizarVehiculo(SistemaTaller* s, int id, Vehiculo datos) {
    Vehiculo* vehiculo = buscarVehiculoPorID(s, id);
    if (vehiculo == nullptr) return false;

    if (datos.anio < 1950 || datos.anio > 2027) return false;
    if (esTextoVacio(datos.marca) || esTextoVacio(datos.modelo)) return false;
    if (esTextoVacio(datos.nombrePropietario)) return false;
    if (!esCedulaValida(datos.cedulaPropietario)) return false;

    copiarTexto(vehiculo->marca, datos.marca, sizeof(vehiculo->marca));
    copiarTexto(vehiculo->modelo, datos.modelo, sizeof(vehiculo->modelo));
    vehiculo->anio = datos.anio;
    copiarTexto(vehiculo->nombrePropietario, datos.nombrePropietario, sizeof(vehiculo->nombrePropietario));
    copiarTexto(vehiculo->cedulaPropietario, datos.cedulaPropietario, sizeof(vehiculo->cedulaPropietario));
    copiarTexto(vehiculo->telefonoPropietario, datos.telefonoPropietario, sizeof(vehiculo->telefonoPropietario));
    return true;
}

// Elimina un vehiculo del arreglo, solo si no tiene ordenes registradas
// (en ningun estado). Los vehiculos que estaban despues se corren una
// posicion hacia la izquierda para no dejar huecos
bool eliminarVehiculo(SistemaTaller* s, int id) {
    int posicion = -1;
    for (int i = 0; i < s->numVehiculos; i++) {
        if (s->vehiculos[i].id == id) {
            posicion = i;
            break;
        }
    }
    if (posicion == -1) return false;
    if (contarOrdenesDeVehiculo(s, id) > 0) return false;

    for (int i = posicion; i < s->numVehiculos - 1; i++) {
        s->vehiculos[i] = s->vehiculos[i + 1];
    }
    s->numVehiculos--;
    return true;
}
