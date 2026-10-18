#ifndef UTILIDADES_H
#define UTILIDADES_H

// Herramientas que usan todos los modulos de logica
// Ninguna usa cin ni cout

// Texto
void copiarTexto(char* destino, const char* origen, int tamanoDestino);
void convertirAMayusculas(char* texto);
bool esTextoVacio(const char* texto);
bool contieneSinMayusculas(const char* texto, const char* subcadena);

// Fechas
void obtenerFechaActual(char* destino); // destino debe tener al menos 11 chars
bool esFechaValida(const char* fecha); // YYYY-MM-DD con dia/mes reales

// Datos del negocio
bool esPlacaValida(const char* placa);
bool esCedulaValida(const char* cedula);
bool esEspecialidadValida(const char* especialidad);

#endif
