#ifndef ENTRADA_H
#define ENTRADA_H

// Funciones para leer datos del usuario de forma segura
// Las que leen datos de un registro devuelven false si el usuario
// escribe CANCELAR, para que el menu se salga sin guardar nada

// Pide un numero entero. Si escribe texto, lo vuelve a pedir
bool leerEntero(const char* mensaje, int* destino);

// Pide un numero con decimales (precios, mano de obra)
bool leerFlotante(const char* mensaje, float* destino);

// Pide un texto y lo copia en un char[] sin pasarse de su tamano
bool leerTexto(const char* mensaje, char* destino, int tamanoDestino);

// Pregunta S/N. Devuelve true si responde S
bool confirmar(const char* mensaje);

// Pide una opcion de menu entre minimo y maximo. Repite hasta que sea valida
int leerOpcion(const char* mensaje, int minimo, int maximo);

#endif
