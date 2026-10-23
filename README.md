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

La idea es que la lógica no sepa nada de cómo se ve el programa. Las funciones de `logica/` reciben datos, hacen lo suyo y devuelven algo, y ya. Las de `presentacion/` son las que preguntan, llaman a la lógica y deciden qué mensaje mostrar.

Así si algún día el programa se hiciera con ventanas o en una página web, la lógica se podría usar igualita y solo cambiaría la presentación. Además nos sirvió para repartirnos el trabajo.

Pero si la lógica no puede usar cout, ¿cómo le avisa al menú que algo falló y por qué? Para eso hicimos el enum `Resultado` (está en `logica/resultados.h`). Las funciones que pueden fallar por varias razones devuelven uno de estos valores y el menú, según cuál sea, muestra el mensaje que toca:

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

## Las firmas que diseñamos

Las de vehículos son las que venían en el enunciado. Las de mecánicos, repuestos y órdenes las hicimos nosotros copiando ese estilo, pero en vez de bool usamos `Resultado`. Con un bool el menú solo sabe que algo falló pero no por qué. Nos pasó con `eliminarVehiculo`, que devuelve bool: en el menú tuvimos que buscar el carro antes para saber si el error era que no existía o que tenía órdenes.

Más o menos la regla que seguimos fue:

- Lo que cambia algo (registrar, cerrar, anular...) devuelve `Resultado`.
- Buscar uno solo devuelve un puntero a ese elemento dentro del arreglo, o `nullptr` si no está.
- Listar o buscar varios devuelve un arreglo nuevo de punteros (`Entidad**`) y la cantidad va por un `int*`, porque el return ya está ocupado con el arreglo. El que la llama tiene que hacer `delete[]` de ese arreglo (solo del arreglo, no de las cosas a las que apunta).
- Lo que solo calcula o pregunta algo de sí o no devuelve un `float`, `int` o `bool` normal porque no puede fallar.

### Mecánicos

```cpp
Resultado registrarMecanico(SistemaTaller* s, Mecanico datos);
Mecanico* buscarMecanicoPorID(SistemaTaller* s, int id);
Mecanico* buscarMecanicoPorCedula(SistemaTaller* s, const char* cedula);
Mecanico** listarMecanicos(SistemaTaller* s, int* cantidad);
int contarOrdenesAbiertasMecanico(SistemaTaller* s, int idMecanico);
```

`registrarMecanico` puede fallar porque la cédula ya existe o porque algún dato está malo (una especialidad que no existe, la fecha mal escrita), por eso `Resultado`. La de buscar por cédula la usamos más que todo por dentro para no repetir cédulas. `contarOrdenesAbiertasMecanico` solo cuenta, la usa `abrirOrden` para lo del máximo de 3.

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

`buscarRepuestos` busca por un pedazo del código o del nombre, por eso puede devolver varios. `eliminarRepuesto` da `ERROR_EN_USO` si el repuesto sale en alguna orden, porque si se borrara esas órdenes se quedarían con un ID que ya no existe. `tieneBajoStock` recibe el repuesto directo y no el sistema porque solo compara su stock con su mínimo.

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

`abrirOrden` es la que más cosas tiene que revisar (que existan el carro y el mecánico, que el carro no tenga otra orden abierta, que el mecánico tenga cupo, que el km no sea menor), por eso `Resultado`. Como además necesitábamos saber qué número le tocó a la orden nueva, ese va por el puntero `idNuevaOrden`.

El total no se guarda en la orden, se calcula cada vez con `calcularTotalOrden` (cantidad × precioUnitario de cada línea, más la mano de obra). Así nunca queda desactualizado.

`buscarDetalleEnOrden` la usa `agregarRepuestoAOrden` para ver si ese repuesto ya estaba en la orden y sumarle ahí en vez de crear otra línea. `contarOrdenesDeVehiculo` la usa el menú de vehículos para decir cuántas órdenes tiene un carro cuando no se puede eliminar.

## Pruebas

En `pruebas/entrada.txt` está todo lo que uno escribiría a mano para hacer los 8 casos de uso del enunciado, uno detrás del otro. Hay que correrlo completo porque cada caso usa lo que dejó el anterior (el programa siempre arranca vacío). Lo que mostró el programa quedó en `pruebas/salida.txt`.

| Caso | Líneas | Qué hace |
|---|---|---|
| 1 | 1-39 | Registra los 2 carros de María González (AB123CD y XY987ZW) y uno de Carlos Ramírez. Busca "gonz" y salen los 2 de María, busca la placa XY987ZW y lista todo |
| 2 | 40-78 | Registra 4 repuestos, el filtro de aceite queda con stock igual al mínimo y sale BAJO STOCK. Le entran 10 y ya no sale marcado |
| 3 | 79-122 | Registra a José Pérez y abre la orden #1 para AB123CD con 45000 km. Le mete 2 pastillas y 1 líquido, después 1 pastilla más (queda una sola línea con 3). Quita el líquido y su stock vuelve a 10. Se cierra con 40 de mano de obra: total 115 |
| 4 | 123-142 | Cambia el precio de las pastillas a 5. La orden #1 sigue en 115. Abre la orden #2 para XY987ZW y las pastillas ya salen al precio nuevo |
| 5 | 143-151 | Anula la orden #2, queda ANULADA y el stock de pastillas vuelve a 17 que era lo que había |
| 6 | 152-262 | Las 8 validaciones: placa repetida, segunda orden abierta para el mismo carro, cuarta orden abierta para José, km menor, más bujías de las que hay, meterle algo a la orden #1 que ya está cerrada, eliminar un carro con órdenes y eliminar un repuesto usado. Todas salen rechazadas |
| 7 | 263-281 | Lista los 5 carros con sus datos bien. Le mete pastillas, filtro y bujía a la orden #4 (3 repuestos distintos) y el detalle sale con las 3 líneas y total 28 |
| 8 | 282-291 | Empieza a registrar el carro HY589LK y pone CANCELAR en el kilometraje. Después lo busca por placa y no aparece |

Sobre el caso 7: el quinto carro (GA123TO) se registra en el caso 6, en las líneas 218-229, y ahí es donde el arreglo de vehículos pasa de 4 a 8. En el caso 7 solo se listan los 5 para ver que después de crecer siguen todos bien.

## Preguntas de análisis

**1. agregarVehiculo retorna un puntero a un elemento de s->vehiculos. Si guardan ese puntero en una variable, registran varios vehículos más y luego lo usan, ¿qué puede ocurrir y por qué?**

El puntero apunta a una posición dentro de `s->vehiculos`. Si después registramos más carros y el arreglo se llena, `asegurarCapacidadVehiculos` crea uno nuevo del doble de tamaño, copia todos los carros ahí y borra el viejo con `delete[]`. El problema es que nuestro puntero sigue apuntando al arreglo viejo, que ya no existe. Queda como un puntero colgante, y si lo usamos podemos leer basura o el programa se puede caer.

Ojo que no es una fuga de memoria, porque la memoria vieja sí se liberó. El problema es al revés: estamos usando memoria que ya se liberó. Tampoco sirve ponerle `nullptr`, porque la función que agranda el arreglo no sabe que nuestra variable existe. Lo seguro es guardar el ID del carro y volver a buscarlo con `buscarVehiculoPorID` cuando lo necesitemos, porque el ID no cambia aunque el carro se mueva de lugar.

**2. ¿Por qué DetalleOrden guarda precioUnitario en lugar de consultar el precio del repuesto cada vez que se calcula el total?**

Porque el precio de un repuesto puede cambiar y eso no debería dañar las órdenes que ya se hicieron. El total de una orden no está guardado en ningún lado, se calcula cada vez que se pide. Si se calculara con el precio actual del repuesto, cada vez que cambiáramos un precio cambiarían también los totales de órdenes viejas, aunque ya estén cerradas y el cliente ya haya pagado.

Es como una factura del súper: si mañana sube la harina, la factura de ayer sigue diciendo lo mismo. Por eso cuando se agrega un repuesto a la orden se copia el precio que tiene en ese momento a `precioUnitario`, y el total siempre se saca con esa copia. Eso es lo que se prueba en el caso 4.

**3. Cuando se redimensiona el arreglo de órdenes, ¿qué ocurre con el arreglo de detalles de cada orden? ¿Se copia, se comparte o se pierde?**

La orden no tiene los detalles adentro, solo tiene un puntero (`detalles`) con la dirección donde están. Cuando `asegurarCapacidadOrdenes` crea el arreglo nuevo y hace `nuevoArreglo[i] = s->ordenes[i]`, se copia la orden completa, pero del puntero lo que se copia es solo la dirección, no las líneas.

Entonces los detalles no se copian (nunca se hace un `new` de detalles nuevos) y tampoco se pierden (el `delete[] s->ordenes` borra solo el arreglo de órdenes, nunca los detalles). Lo que pasa es que se comparten por un momento: la orden vieja y la nueva apuntan al mismo arreglo de detalles. Después se borra el arreglo viejo y la orden nueva se queda como la única dueña.

Por eso en esa función NO se puede hacer `delete[]` de los detalles. Si se hiciera, la orden nueva quedaría apuntando a memoria liberada. Los detalles solo se borran en `liberarSistema` al salir, y ahí primero se borran los de cada orden y después el arreglo de órdenes, porque si fuera al revés se perderían las direcciones y quedaría memoria sin liberar.
