#ifndef MENU_MECANICOS_H
#define MENU_MECANICOS_H

#include "../estructuras.h"

// Submenu de la opcion 2 del menu principal
void menuMecanicos(SistemaTaller* s);

void menuRegistrarMecanico(SistemaTaller* s);
void menuBuscarMecanico(SistemaTaller* s);
void menuListarMecanicos(SistemaTaller* s);

// Reciben s para poder mostrar cuantas ordenes ABIERTAS tiene cada mecanico
void mostrarMecanico(SistemaTaller* s, Mecanico* mecanico);
void mostrarListaMecanicos(SistemaTaller* s, Mecanico** mecanicos, int cantidad);

#endif
