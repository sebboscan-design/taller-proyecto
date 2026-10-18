#ifndef MENU_REPUESTOS_H
#define MENU_REPUESTOS_H

#include "../estructuras.h"

// Submenu de la opcion 3 del menu principal
void menuRepuestos(SistemaTaller* s);

void menuRegistrarRepuesto(SistemaTaller* s);
void menuBuscarRepuesto(SistemaTaller* s);
void menuEntradaMercancia(SistemaTaller* s);
void menuCambiarPrecio(SistemaTaller* s);
void menuEliminarRepuesto(SistemaTaller* s);
void menuListarRepuestos(SistemaTaller* s);
void mostrarRepuesto(Repuesto* repuesto);
void mostrarListaRepuestos(Repuesto** repuestos, int cantidad);

#endif
