# Sistema de gestión de taller

Proyecto 1 de Programación 2 (2026-C)

## De qué trata

Es un programa de consola hecho en C++ para manejar un taller mecánico. Básicamente se registran los carros que llegan (con los datos del dueño), los mecánicos y los repuestos que hay en el inventario.

Lo más importante son las órdenes de servicio. Cuando llega un carro se le abre una orden, se le pone un mecánico y se le van metiendo los repuestos que se usan, y ahí mismo se descuentan del stock. Cuando terminan el trabajo se cierra la orden con lo que costó la mano de obra. Si algo salió mal la orden se puede anular y los repuestos vuelven al inventario.

Todo queda en memoria mientras el programa está abierto. No usamos vector ni nada de la STL (no estaba permitido), así que todo son arreglos con `new` que se agrandan cuando se llenan. Cuando se sale del programa se libera todo y los datos se pierden.

## Integrantes

- Sebastián Boscán - C.I. 32.741.341
- Camila Velasquez - C.I. 33.319.134

Sebastián hizo la parte de lógica y Camila la de presentación (los menús). Después entre los dos corregimos los menús para que encajaran con las funciones de lógica y armamos las pruebas.

## Cómo está organizado

Separamos el código en dos carpetas:

- `logica/`: aquí van las reglas del taller (validar, guardar, buscar, sacar totales). Nada de aquí usa cin ni cout.
- `presentacion/`: los menús y todo lo que le pide algo al usuario o le muestra algo.

Aparte están `main.cpp` que tiene el menú principal, `estructuras.h` con los structs que nos dio el profe, y la carpeta `pruebas/` con los archivos para correr los casos de uso.

## Compilar y correr

Hay que tener g++ instalado. Desde la carpeta del proyecto:

```
g++ main.cpp logica/*.cpp presentacion/*.cpp -o taller
./taller
```

Para correr los casos de prueba de una vez sin tener que escribir nada:

```
./taller < pruebas/entrada.txt
```

o si se quiere guardar lo que sale en salida.txt:

```
./taller < pruebas/entrada.txt > pruebas/salida.txt
```

Ojo: en PowerShell el `<` no sirve, hay que hacerlo asi:

```
.\taller.exe
Get-Content pruebas\entrada.txt | .\taller.exe
```

Tip: si en medio de un registro te equivocas puedes escribir CANCELAR y no se guarda nada.

## Cómo se conectan las estructuras

Todo cuelga de un solo struct, `SistemaTaller`. Ese struct tiene un arreglo dinámico por cada cosa del taller, y cada arreglo tiene su contador (cuántos hay) y su capacidad (cuántos caben).

```
SistemaTaller
 ├── taller            (nombre, rif... solo se carga una vez)
 ├── vehiculos  ──►  [ Vehiculo | Vehiculo | Vehiculo | ... ]
 ├── mecanicos  ──►  [ Mecanico | Mecanico | ... ]
 ├── repuestos  ──►  [ Repuesto | Repuesto | Repuesto | ... ]
 └── ordenes    ──►  [ OrdenServicio | OrdenServicio | ... ]
                            │
                            │ cada orden tiene su propio arreglo
                            ▼
                     detalles ──► [ DetalleOrden | DetalleOrden | ... ]
```

Las entidades no se guardan unas dentro de otras, se relacionan por ID:

```
OrdenServicio
  idVehiculo  ──────►  Vehiculo     (el carro que se esta atendiendo)
  idMecanico  ──────►  Mecanico     (el que hace el trabajo)
  detalles[ ]
     └── DetalleOrden
           idRepuesto  ──►  Repuesto   (que repuesto se uso)
           cantidad
           precioUnitario   (copia del precio cuando se agrego)
```

Lo importante es la parte de orden → detalles. La orden no tiene los detalles adentro, solo tiene un puntero (`detalles`) que apunta a otro arreglo en otra parte de la memoria, con su propio `numDetalles` y `capacidadDetalles`. Ese arreglo empieza con capacidad 2 y se duplica cuando se llena, igual que los arreglos grandes. Por eso al salir del programa primero hay que liberar los detalles de cada orden y despues el arreglo de ordenes, si no se pierde la direccion de los detalles y queda memoria sin liberar.

Usamos IDs en vez de punteros para relacionar las cosas porque los arreglos se mueven de lugar cuando crecen. Un puntero a un vehiculo podria quedar apuntando a memoria que ya se libero, pero el ID siempre es el mismo.

## Por qué separamos en capas

La idea es que la lógica no sepa nada de cómo se ve el programa. Las funciones de `logica/` reciben datos, hacen su trabajo y devuelven un resultado, y ya. Las de `presentacion/` son las que preguntan cosas, llaman a la lógica y deciden qué mensaje mostrar.

Así si mañana el programa se hiciera con ventanas o en una página web, la lógica se podría usar igualito y solo habría que cambiar la presentación. También es más fácil de probar y de repartirse el trabajo, que fue lo que hicimos: uno hizo lógica y la otra presentación.

El problema es que si la lógica no puede usar cout, ¿cómo le avisa al menú que algo salió mal y por qué? Para eso hicimos el enum `Resultado` (en `logica/resultados.h`). Cada función que puede fallar por varias razones devuelve uno de estos valores y el menú, según cuál sea, muestra el mensaje que toca:

```
OK
ERROR_NO_EXISTE                    el ID o código no está registrado
ERROR_DATO_INVALIDO                precio, cantidad, fecha, etc. no válidos
ERROR_DUPLICADO                    cédula o código repetido
ERROR_ESTADO_INVALIDO              la orden no está en el estado que se necesita
ERROR_VEHICULO_CON_ORDEN_ABIERTA   el carro ya tiene una orden abierta
ERROR_MECANICO_SIN_CUPO            el mecánico ya tiene 3 órdenes abiertas
ERROR_KILOMETRAJE_MENOR            el km es menor al último registrado
ERROR_STOCK_INSUFICIENTE           no hay suficientes unidades
ERROR_EN_USO                       el repuesto aparece en alguna orden
ERROR_ORDEN_VACIA                  cerrar sin repuestos y con mano de obra 0
```

## Firmas que diseñamos

Para vehículos usamos las firmas que venían en el enunciado. Para mecánicos, repuestos y órdenes las hicimos nosotros siguiendo el mismo estilo, pero cambiamos los bool por `Resultado`. Con un bool el menú solo sabe que falló, no por qué (por ejemplo `eliminarVehiculo` devuelve bool y en el menú tuvimos que buscar el vehículo antes para saber si el error era que no existía o que tenía órdenes).

En general seguimos estas reglas:

- **Operaciones que cambian algo** (registrar, cerrar, anular...) devuelven `Resultado`, para saber exactamente qué falló.
- **Búsquedas de uno solo** devuelven un puntero al elemento dentro del arreglo, o `nullptr` si no existe. Así se puede leer o modificar directo sin copiar nada.
- **Listados y búsquedas de varios** devuelven `Entidad**` (un arreglo nuevo de punteros) y la cantidad por un `int*`, porque el return ya se usa para el arreglo. El que llama tiene que hacer `delete[]` de ese arreglo (solo del arreglo, no de las entidades).
- **Cálculos y preguntas de sí/no** devuelven `float`, `int` o `bool` directamente porque no pueden fallar.

### Mecánicos

```cpp
Resultado registrarMecanico(SistemaTaller* s, Mecanico datos);
Mecanico* buscarMecanicoPorID(SistemaTaller* s, int id);
Mecanico* buscarMecanicoPorCedula(SistemaTaller* s, const char* cedula);
Mecanico** listarMecanicos(SistemaTaller* s, int* cantidad);
int contarOrdenesAbiertasMecanico(SistemaTaller* s, int idMecanico);
```

`registrarMecanico` puede fallar por cédula repetida o por algún dato malo (especialidad que no existe, fecha mal escrita), por eso devuelve `Resultado`. `buscarMecanicoPorCedula` la usamos sobre todo por dentro, para no repetir cédulas. `contarOrdenesAbiertasMecanico` devuelve un int porque solo cuenta, y la usa `abrirOrden` para el límite de 3.

### Repuestos

```cpp
Resultado registrarRepuesto(SistemaTaller* s, Repuesto datos);
Repuesto* buscarRepuestoPorID(SistemaTaller* s, int id);
Repuesto* buscarRepuestoPorCodigo(SistemaTaller* s, const char* codigo);
Repuesto** buscarRepuestos(SistemaTaller* s, const char* texto, int* cantidad);
Repuesto** listarRepuestos(SistemaTaller* s, int* cantidad);
Resultado registrarEntradaMercancia(SistemaTaller* s, int idRepuesto, int cantidad);
Resultado cambiarPrecioRepuesto(SistemaTaller* s, int idRepuesto, float nuevoPrecio);
Resultado eliminarRepuesto(SistemaTaller* s, int idRepuesto);
bool tieneBajoStock(Repuesto* repuesto);
bool repuestoUsadoEnOrdenes(SistemaTaller* s, int idRepuesto);
```

`buscarRepuestos` busca por parte del código o del nombre, por eso puede devolver varios. `eliminarRepuesto` devuelve `ERROR_EN_USO` si el repuesto sale en alguna orden, porque si se borrara esas órdenes quedarían con un ID que ya no existe. `tieneBajoStock` recibe directo el repuesto (no el sistema) porque solo compara su stock con su mínimo.

### Órdenes

```cpp
Resultado abrirOrden(SistemaTaller* s, int idVehiculo, int idMecanico,
                     int kilometrajeIngreso, const char* descripcionFalla, int* idNuevaOrden);
Resultado agregarRepuestoAOrden(SistemaTaller* s, int idOrden, int idRepuesto, int cantidad);
Resultado quitarRepuestoDeOrden(SistemaTaller* s, int idOrden, int idRepuesto);
Resultado cerrarOrden(SistemaTaller* s, int idOrden, float manoObra);
Resultado anularOrden(SistemaTaller* s, int idOrden);
float calcularSubtotalRepuestos(OrdenServicio* orden);
float calcularTotalOrden(OrdenServicio* orden);
OrdenServicio* buscarOrdenPorID(SistemaTaller* s, int id);
OrdenServicio* buscarOrdenAbiertaDeVehiculo(SistemaTaller* s, int idVehiculo);
DetalleOrden* buscarDetalleEnOrden(OrdenServicio* orden, int idRepuesto);
int contarOrdenesDeVehiculo(SistemaTaller* s, int idVehiculo);
OrdenServicio** listarOrdenes(SistemaTaller* s, int* cantidad);
OrdenServicio** listarOrdenesPorEstado(SistemaTaller* s, const char* estado, int* cantidad);
OrdenServicio** listarOrdenesPorVehiculo(SistemaTaller* s, int idVehiculo, int* cantidad);
```

`abrirOrden` es la que más puede fallar (vehículo o mecánico que no existe, carro con otra orden abierta, mecánico sin cupo, km menor), por eso devuelve `Resultado`. Como también necesitábamos saber el número de la orden nueva, ese va por el puntero `idNuevaOrden`.

Los totales no se guardan en la orden, se calculan cada vez con `calcularTotalOrden` sumando cantidad × precioUnitario de cada línea más la mano de obra. Así nunca se desincronizan.

`buscarDetalleEnOrden` la usa `agregarRepuestoAOrden` para ver si el repuesto ya estaba en la orden y sumarle a esa línea en vez de crear otra. `contarOrdenesDeVehiculo` la usa el menú de vehículos para decir cuántas órdenes tiene un carro cuando no se puede eliminar.
