#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

// 1.1 Estructura Taller (contexto del sistema)
struct Taller {
    char nombre[100];     // Nombre del taller
    char rif[20];         // RIF del taller
    char direccion[200];  // Dirección física
    char telefono[20];    // Teléfono de contacto
};

// 1.2 Estructura Vehiculo
struct Vehiculo {
    int id;                        // Identificador único (autoincremental)
    char placa[10];                // Placa del vehículo (única en el sistema)
    char marca[50];                // Ej: "Toyota"
    char modelo[50];               // Ej: "Corolla"
    int anio;                      // Año de fabricación
    int kilometraje;               // Último kilometraje registrado
    char nombrePropietario[100];   // Nombre completo del propietario
    char cedulaPropietario[20];    // Cédula o RIF del propietario
    char telefonoPropietario[20];  // Teléfono de contacto
    char fechaRegistro[11];        // Formato: YYYY-MM-DD
};

// 1.3 Estructura Mecanico
struct Mecanico {
    int id;                 // Identificador único (autoincremental)
    char nombre[100];       // Nombre completo
    char cedula[20];        // Cédula de identidad
    char especialidad[20];  // "MOTOR", "FRENOS", "ELECTRICIDAD",
                            // "SUSPENSION" o "GENERAL"
    char fechaIngreso[11];  // Formato: YYYY-MM-DD
};

// 1.4 Estructura Repuesto
struct Repuesto {
    int id;                  // Identificador único (autoincremental)
    char codigo[20];         // Código del repuesto (único)
    char nombre[100];        // Ej: "Pastillas de freno delanteras"
    char marca[50];          // Marca del repuesto
    float precio;            // Precio unitario vigente
    int stock;               // Unidades disponibles
    int stockMinimo;         // Umbral para alerta de reposición
    char fechaRegistro[11];  // Formato: YYYY-MM-DD
};

// 1.5 Estructuras DetalleOrden y OrdenServicio
struct DetalleOrden {
    int idRepuesto;        // ID del repuesto utilizado
    int cantidad;          // Unidades utilizadas
    float precioUnitario;  // Precio del repuesto AL MOMENTO de agregarlo
};

struct OrdenServicio {
    int id;                      // Identificador único (autoincremental)
    int idVehiculo;              // Vehículo atendido
    int idMecanico;              // Mecánico responsable
    char fechaApertura[11];      // Formato: YYYY-MM-DD
    char fechaCierre[11];        // Vacía mientras la orden esté ABIERTA
    char estado[12];             // "ABIERTA", "CERRADA" o "ANULADA"
    char descripcionFalla[200];  // Falla reportada por el propietario
    int kilometrajeIngreso;      // Kilometraje al abrir la orden
    float manoObra;              // Costo de mano de obra

    // Arreglo dinámico propio de la orden
    DetalleOrden* detalles;
    int numDetalles;
    int capacidadDetalles;
};

// 1.6 Estructura Principal: SistemaTaller
struct SistemaTaller {
    Taller taller;  // Datos del taller (único, no es arreglo)

    Vehiculo* vehiculos;    int numVehiculos;  int capacidadVehiculos;
    Mecanico* mecanicos;    int numMecanicos;  int capacidadMecanicos;
    Repuesto* repuestos;    int numRepuestos;  int capacidadRepuestos;
    OrdenServicio* ordenes; int numOrdenes;    int capacidadOrdenes;

    // Contadores para IDs autoincrementales
    int siguienteIdVehiculo;
    int siguienteIdMecanico;
    int siguienteIdRepuesto;
    int siguienteIdOrden;
};

#endif
