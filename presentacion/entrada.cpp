#include "entrada.h"
#include "../logica/utilidades.h"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

// Lee una linea completa con getline. Si es CANCELAR devuelve false
// Si no es un numero valido, muestra un aviso y la vuelve a pedir
bool leerEntero(const char* mensaje, int* destino) {
   string linea;
    while (true) {
        cout << mensaje;
        if (!getline(cin, linea)) {
            return false; // Evita ciclo infinito si se acaba el archivo (.eof())
        }
        if (linea == "CANCELAR") {
            return false;
        }
        
        try {
            size_t idx;
            int valor = stoi(linea, &idx);
            // Verifica que no haya caracteres extra después del número
            if (idx == linea.length()) {
                *destino = valor;
                return true;
            }
        } catch (...) {
            // Captura stoi inválido o fuera de rango
        }
        cout << "Error: Por favor, ingrese un numero entero valido.\n";
    }
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
   string linea;
    while (true) {
        cout << mensaje;
        if (!getline(cin, linea)) {
            return false;
        }
        if (linea == "CANCELAR") {
            return false;
        }
        
        try {
            size_t idx;
            float valor = stof(linea, &idx);
            if (idx == linea.length()) {
                *destino = valor;
                return true;
            }
        } catch (...) {
        }
        cout << "Error: Por favor, ingrese un numero decimal valido.\n";
    }
    return false;
}
// Lee una linea de texto. Si es CANCELAR devuelve false
// std::string solo se usa aqui para leer; despues se copia al char[]
// con copiarTexto (de utilidades.h)
bool leerTexto(const char* mensaje, char* destino, int tamanoDestino) {
    string linea;
    cout << mensaje;
    if (!getline(cin, linea)) {
        return false;
    }
    if (linea == "CANCELAR") {
        return false;
    }
    
    // Se usa la función utilitaria solicitada
    copiarTexto(destino, linea.c_str(), tamanoDestino);
    return true;
}


// Acepta solo S o N (mayuscula o minuscula). Repite hasta que sea una de las dos
bool confirmar(const char* mensaje) {
   string linea;
    while (true) {
        cout << mensaje;
        if (!getline(cin, linea)) {
            return false; 
        }
        if (linea.length() == 1) {
            char c = tolower(linea[0]);
            if (c == 's') return true;
            if (c == 'n') return false;
        }
        cout << "Error: Ingrese 'S' para si o 'N' para no.\n";
    }
    return false;
}

// Pide un numero y lo repite hasta que este entre minimo y maximo
int leerOpcion(const char* mensaje, int minimo, int maximo) {
   int opcion;
    string mensajeRango = string(mensaje) + " (" + toat(minimo) + "-" + to_string(maximo) + "): "; // Opcional por estetica
    
    // Reutiliza leerEntero para evitar duplicar lógica de validación
    while (leerEntero(mensaje, &opcion)) {
        if (opcion >= minimo && opcion <= maximo) {
            return opcion;
        }
        cout << "Error: La opcion debe estar entre " << minimo << " y " << maximo << ".\n";
    }
    return minimo - 1; // Retorna un valor fuera de rango si se cancela o termina el archivo
}
    return 0;
}
