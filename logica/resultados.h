#ifndef RESULTADOS_H
#define RESULTADOS_H

// Resultado de una operacion de logica
// Un bool solo dice si algo fallo; este tipo dice tambien POR QUE fallo,
// para que el menu pueda mostrar el mensaje de error correcto
enum Resultado {
    OK,
    ERROR_NO_EXISTE, // el ID o codigo no esta registrado
    ERROR_DATO_INVALIDO, // precio, cantidad, especialidad, etc. fuera de lo permitido
    ERROR_DUPLICADO, // cedula o codigo ya registrado
    ERROR_ESTADO_INVALIDO, // la orden no esta en el estado que la operacion necesita
    ERROR_VEHICULO_CON_ORDEN_ABIERTA, // el vehiculo ya tiene una orden ABIERTA
    ERROR_MECANICO_SIN_CUPO, // el mecanico ya tiene 3 ordenes ABIERTAS
    ERROR_KILOMETRAJE_MENOR, // km de ingreso menor al ultimo registrado
    ERROR_STOCK_INSUFICIENTE, // no hay unidades suficientes del repuesto
    ERROR_EN_USO, // el repuesto aparece en alguna orden
    ERROR_ORDEN_VACIA // cerrar sin repuestos y con mano de obra 0
};

#endif
