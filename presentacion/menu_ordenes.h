#ifndef MENU_ORDENES_H
#define MENU_ORDENES_H

#include "../estructuras.h"

// Submenu de la opcion 4 del menu principal (formato de la seccion 5 del PDF)
void menuOrdenes(SistemaTaller* s);

void menuAbrirOrden(SistemaTaller* s);
void menuAgregarRepuestoOrden(SistemaTaller* s);
void menuQuitarRepuestoOrden(SistemaTaller* s);
void menuCerrarOrden(SistemaTaller* s);
void menuAnularOrden(SistemaTaller* s);
void menuVerDetalleOrden(SistemaTaller* s);
void menuListarOrdenes(SistemaTaller* s);

// Reciben s para poder mostrar la placa del vehiculo, el nombre del
// mecanico y los datos de cada repuesto en lugar de solo IDs
void mostrarDetalleOrden(SistemaTaller* s, OrdenServicio* orden);
void mostrarListaOrdenes(SistemaTaller* s, OrdenServicio** ordenes, int cantidad);

#endif
