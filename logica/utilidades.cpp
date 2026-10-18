#include "utilidades.h"
#include <cstring>
#include <cctype>
#include <ctime>
#include <cstdio>

void copiarTexto(char* destino, const char* origen, int tamanoDestino) {
    // strncpy a secas no siempre cierra el texto con '\0', por eso el ultimo paso
    strncpy(destino, origen, tamanoDestino - 1);
    destino[tamanoDestino - 1] = '\0';
}

void convertirAMayusculas(char* texto) {
    // asi "ab123cd" y "AB123CD" cuentan como la misma placa
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = toupper((unsigned char)texto[i]);
    }
}

bool esTextoVacio(const char* texto) {
    // "   " tampoco sirve como nombre
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isspace((unsigned char)texto[i])) return false;
    }
    return true;
}

bool contieneSinMayusculas(const char* texto, const char* subcadena) {
    // para buscar "gonz" y encontrar a "Maria Gonzalez"
    int largoTexto = strlen(texto);
    int largoSub = strlen(subcadena);
    if (largoSub == 0) return true;

    for (int inicio = 0; inicio + largoSub <= largoTexto; inicio++) {
        bool coincide = true;
        for (int j = 0; j < largoSub; j++) {
            if (tolower((unsigned char)texto[inicio + j]) !=
                tolower((unsigned char)subcadena[j])) {
                coincide = false;
                break;
            }
        }
        if (coincide) return true;
    }
    return false;
}

void obtenerFechaActual(char* destino) {
    // fecha de la compu, se usa al registrar cosas y al cerrar ordenes
    time_t ahora = time(nullptr);
    tm* fecha = localtime(&ahora);
    sprintf(destino, "%04d-%02d-%02d",
            fecha->tm_year + 1900, fecha->tm_mon + 1, fecha->tm_mday);
}

bool esFechaValida(const char* fecha) {
    // formato exacto YYYY-MM-DD: 10 caracteres, guiones en 4 y 7
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit((unsigned char)fecha[i])) return false;
    }

    int anio = (fecha[0]-'0')*1000 + (fecha[1]-'0')*100 + (fecha[2]-'0')*10 + (fecha[3]-'0');
    int mes = (fecha[5]-'0')*10 + (fecha[6]-'0');
    int dia = (fecha[8]-'0')*10 + (fecha[9]-'0');

    if (anio < 1900 || mes < 1 || mes > 12 || dia < 1) return false;

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool esBisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
    if (esBisiesto) diasPorMes[1] = 29; // el 29 de febrero solo en bisiestos

    return dia <= diasPorMes[mes - 1];
}

bool esPlacaValida(const char* placa) {
    // placas venezolanas: 6 o 7 caracteres, solo letras y numeros
    int largo = strlen(placa);
    if (largo < 6 || largo > 7) return false;
    for (int i = 0; i < largo; i++) {
        if (!isalnum((unsigned char)placa[i])) return false;
    }
    return true;
}

bool esCedulaValida(const char* cedula) {
    // se acepta "12345678" o con prefijo "V-12345678", "E-...", "J-..."
    int inicio = 0;
    char primera = toupper((unsigned char)cedula[0]);
    if ((primera == 'V' || primera == 'E' || primera == 'J') && cedula[1] == '-') {
        inicio = 2;
    }

    if (cedula[inicio] == '\0') return false; // "V-" solo no vale
    for (int i = inicio; cedula[i] != '\0'; i++) {
        if (!isdigit((unsigned char)cedula[i])) return false;
    }
    return true;
}

bool esEspecialidadValida(const char* especialidad) {
    // las unicas que maneja el taller segun el enunciado
    const char* permitidas[] = {"MOTOR", "FRENOS", "ELECTRICIDAD", "SUSPENSION", "GENERAL"};
    for (int i = 0; i < 5; i++) {
        if (strcmp(especialidad, permitidas[i]) == 0) return true;
    }
    return false;
}
