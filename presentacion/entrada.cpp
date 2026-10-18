#include "entrada.h"
#include "../logica/utilidades.h"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

// Lee una linea completa con getline. Si es CANCELAR devuelve false
// Si no es un numero valido, muestra un aviso y la vuelve a pedir
bool leerEntero(const char* mensaje, int* destino) {
    // TODO: escribir la funcion
    // Ojo: si se acaba la entrada (cin.eof()), devolver false para no
    // quedar en un ciclo infinito cuando se usa ./taller < entrada.txt
    return false;
}

// Igual que leerEntero, pero para numeros con decimales
bool leerFlotante(const char* mensaje, float* destino) {
    // TODO: escribir la funcion
    return false;
}

// Lee una linea de texto. Si es CANCELAR devuelve false
// std::string solo se usa aqui para leer; despues se copia al char[]
// con copiarTexto (de utilidades.h)
bool leerTexto(const char* mensaje, char* destino, int tamanoDestino) {
    // TODO: escribir la funcion
    return false;
}

// Acepta solo S o N (mayuscula o minuscula). Repite hasta que sea una de las dos
bool confirmar(const char* mensaje) {
    // TODO: escribir la funcion
    return false;
}

// Pide un numero y lo repite hasta que este entre minimo y maximo
int leerOpcion(const char* mensaje, int minimo, int maximo) {
    // TODO: escribir la funcion
    return 0;
}
