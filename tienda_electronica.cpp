#include <iostream>
#include <cstring>  // Para usar funciones de cadenas (strcpy, strcmp, strlen)
#include <fstream>  // Para el manejo de Archivos (Criterio 6)
#include <locale>   // Necesario para manejar la configuracion regional (tildes, ñ)
#include <cstdio>   // Necesario para usar sscanf y sprintf (util en modificacion y facturacion)
#include <cstdlib>  // Necesario para usar atof, atoi y exit (conversion de cadena a numero y memoria)
#include <ctime>    // Necesario para obtener la fecha y hora actual

using namespace std;

// --- DEFINICION DE ESTRUCTURAS DE DATOS (REGISTROS) ---
const int MAX_PRODUCTOS = 100;
const int MAX_CLIENTES = 50;
const int MAX_FACTURAS = 50; // Limite de facturas guardadas en memoria

// Estructura para almacenar los detalles de un producto en el inventario.
struct Producto {
    char codigo[10];
    char nombre[50];
    double precio;
    int stock;
    char categoria[20]; 
};

// Estructura para almacenar los detalles de una linea dentro de la factura.
struct DetalleVenta {
    char codProducto[10]; 
    int cantidad;         
    double precioUnitario; 
};

// Estructura para almacenar la cabecera y el cuerpo de una factura.
struct Factura {
    int numeroFactura;      
    char nitCliente[15];    
    char fecha[15];         
    double total;           
    DetalleVenta *detalles; // PUNTERO al arreglo de detalles (Memoria Dinamica)
    int numDetalles;        
};

// Estructura para almacenar la informacion de los clientes.
struct Cliente {
    char nit[15];       
    char nombre[50];    
    char direccion[100]; 
    char telefono[10];  
};

// --- ARREGLOS Y CONTADORES GLOBALES (Concepto: Arreglos) ---

Producto inventario[MAX_PRODUCTOS];  
int numProductos = 0;                

Cliente listaClientes[MAX_CLIENTES]; 
int numClientes = 0;                 

Factura listaFacturas[MAX_FACTURAS]; // Arreglo para guardar las Facturas
int numFacturas = 0;

// Correlativo de Factura (Variable que necesita Persistencia)
int numFacturaActual = 1; 

// --- DECLARACION DE PROTOTIPOS DE FUNCIONES (Concepto: Modularizacion) ---
void menuPrincipal();

// PROTOTIPOS DE PRODUCTOS
void moduloProductos();
void ingresarProducto();
Producto* buscarProducto(const char* codigoBuscado); 
void buscarProductoMenu();
void mostrarProducto(Producto *p);
void modificarProducto();
void eliminarProducto();

// PROTOTIPOS DE CLIENTES
void moduloClientes(); 
void ingresarCliente();
Cliente* buscarCliente(const char* nitBuscado); 
void buscarClienteMenu();
void mostrarCliente(Cliente *c);
void modificarCliente();
void eliminarCliente();

// PROTOTIPOS DE FACTURACION
void moduloFacturacion();
void generarFactura(); 
void listarFacturasMenu();
void obtenerFecha(char *buffer, size_t size);

// PROTOTIPOS DE REPORTES
void moduloReportes();
void listarProductosCompletos(); 
void listarClientesCompletos();

// PROTOTIPOS DE ARCHIVOS Y PERSISTENCIA
void guardarDatos();   
void cargarDatos();
void guardarClientes();
void cargarClientes();
void guardarCorrelativo(); 
void cargarCorrelativo(); 
void guardarFacturas(); // <--- NUEVO
void cargarFacturas();  // <--- NUEVO

// =========================================================================
// FUNCION PRINCIPAL (MAIN)
// =========================================================================

int main() {
    setlocale(LC_ALL, ""); 
    
    // 1. CARGA DE DATOS INICIAL (Persistencia)
    cargarDatos();       
    cargarClientes();    
    cargarCorrelativo(); 
    cargarFacturas();    // <--- Carga las facturas anteriores
    
    menuPrincipal(); 
    
    // 2. GUARDADO FINAL ANTES DE SALIR
    guardarDatos();       
    guardarClientes();    
    guardarCorrelativo(); 
    guardarFacturas();    // <--- Guarda las facturas antes de salir
    
    return 0;
}

// Implementacion del Menu Principal
void menuPrincipal() {
    int opcion; 

    do {
        cout << "\n======================================" << endl;
        cout << "     SISTEMA TIENDA DE ELECTRONICOS" << endl;
        cout << "======================================" << endl;
        cout << "1. Modulo de Productos" << endl;
        cout << "2. Modulo de Clientes" << endl;
        cout << "3. Modulo de Ventas / Facturacion" << endl;
        cout << "4. Modulo de Reportes" << endl; 
        cout << "5. Modulo de Archivos (Guardar Ahora)" << endl;
        cout << "6. Salir del Sistema" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = 0; 
        } else {
             cin.ignore(); 
        }

        switch (opcion) {
            case 1: moduloProductos(); break;
            case 2: moduloClientes(); break;
            case 3: moduloFacturacion(); break;
            case 4: moduloReportes(); break;
            case 5:
                guardarDatos();       
                guardarClientes();    
                guardarCorrelativo();
                guardarFacturas();    // <--- Guardado manual de facturas
                break;
            case 6:
                cout << "\nSaliendo del sistema. ¡Hasta pronto!" << endl;
                break;
            default:
                cout << "\n[ERROR] Opcion no valida. Por favor, intente de nuevo." << endl;
        }

    } while (opcion != 6);
}

// =========================================================================
// MODULO DE REPORTES
// =========================================================================

void moduloReportes() {
    int opcion;
    do {
        cout << "\n--------------------------------------" << endl;
        cout << "      MODULO DE REPORTES" << endl;
        cout << "--------------------------------------" << endl;
        cout << "1. Reporte de Inventario Completo" << endl;
        cout << "2. Reporte de Clientes Registrados" << endl;
        cout << "3. Listar Facturas Registradas" << endl;
        cout << "4. Volver al Menu Principal" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cin.clear(); cin.ignore(10000, '\n'); opcion = 0;
        }
        cin.ignore(); 

        switch (opcion) {
            case 1: listarProductosCompletos(); break;
            case 2: listarClientesCompletos(); break;
            case 3: listarFacturasMenu(); break;
            case 4: cout << "\nVolviendo al Menu Principal..." << endl; break;
            default: cout << "\n[ERROR] Opcion no valida. Intente con un numero del 1 al 4." << endl;
        }
    } while (opcion != 4);
}

// Funcion para listar todo el Inventario (Usa la funcion mostrarProducto)
void listarProductosCompletos() {
    cout << "\n--- REPORTE DE INVENTARIO COMPLETO (" << numProductos << " productos) ---" << endl;
    if (numProductos == 0) {
        cout << "[AVISO] El inventario esta vacio." << endl;
        return;
    }
    
    for (int i = 0; i < numProductos; i++) {
        mostrarProducto(&inventario[i]); 
    }
    cout << "--- FIN DEL REPORTE ---" << endl;
}

// Funcion para listar todos los Clientes (Usa la funcion mostrarCliente)
void listarClientesCompletos() {
    cout << "\n--- REPORTE DE CLIENTES REGISTRADOS (" << numClientes << " clientes) ---" << endl;
    if (numClientes == 0) {
        cout << "[AVISO] No hay clientes registrados." << endl;
        return;
    }
    
    for (int i = 0; i < numClientes; i++) {
        mostrarCliente(&listaClientes[i]); 
    }
    cout << "--- FIN DEL REPORTE ---" << endl;
}


// =========================================================================
// MODULO DE PRODUCTOS
// =========================================================================

void moduloProductos() {
    int opcion;
    do {
        cout << "\n--------------------------------------" << endl;
        cout << "      MODULO DE PRODUCTOS" << endl;
        cout << "--------------------------------------" << endl;
        cout << "1. Ingreso de Nuevo Producto" << endl;
        cout << "2. Modificacion de Producto" << endl;
        cout << "3. Busqueda de Producto" << endl;
        cout << "4. Eliminacion de Producto" << endl;
        cout << "5. Volver al Menu Principal" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cin.clear(); cin.ignore(10000, '\n'); opcion = 0;
        }
        cin.ignore();

        switch (opcion) {
            case 1: ingresarProducto(); break;
            case 2: modificarProducto(); break;
            case 3: buscarProductoMenu(); break;
            case 4: eliminarProducto(); break;
            case 5: cout << "\nVolviendo al Menu Principal..." << endl; break;
            default: cout << "\n[ERROR] Opcion no valida. Intente con un numero del 1 al 5." << endl;
        }
    } while (opcion != 5);
}

void ingresarProducto() {
    if (numProductos >= MAX_PRODUCTOS) {
        cout << "\n[!] ERROR: El inventario esta lleno. No se puede ingresar mas productos." << endl;
        return; 
    }
    cout << "\n--- INGRESO DE NUEVO PRODUCTO ---" << endl;
    Producto nuevoProducto; 
    cout << "Ingrese Codigo (max 9 caracteres): ";
    cin.getline(nuevoProducto.codigo, 10); 
    cout << "Ingrese Nombre del Producto (max 49 caracteres): ";
    cin.getline(nuevoProducto.nombre, 50);
    cout << "Ingrese Categoria (Ej: Laptop, Accesorio): ";
    cin.getline(nuevoProducto.categoria, 20);

    double tempPrecio; 
    cout << "Ingrese Precio: ";
    while (!(cin >> tempPrecio) || tempPrecio <= 0) {
        cout << "[ERROR] Entrada invalida. Ingrese un precio numerico positivo: ";
        cin.clear(); cin.ignore(10000, '\n');
    }
    nuevoProducto.precio = tempPrecio;

    int tempStock; 
    cout << "Ingrese Stock Inicial: ";
    while (!(cin >> tempStock) || tempStock < 0) {
        cout << "[ERROR] Entrada invalida. Ingrese un stock entero no negativo: ";
        cin.clear(); cin.ignore(10000, '\n');
    }
    nuevoProducto.stock = tempStock;
    cin.ignore(); 
    inventario[numProductos] = nuevoProducto;
    numProductos++; 
    cout << "\n[EXITO] Producto '" << nuevoProducto.nombre << "' ingresado. Productos totales: " << numProductos << endl;
}

Producto* buscarProducto(const char* codigoBuscado) {
    for (int i = 0; i < numProductos; i++) {
        if (strcmp(inventario[i].codigo, codigoBuscado) == 0) {
            return &inventario[i];
        }
    }
    return NULL; 
}

void mostrarProducto(Producto *p) {
    cout << "\n--- DETALLE DEL PRODUCTO ---" << endl;
    cout << "Codigo: " << p->codigo << endl;
    cout << "Nombre: " << p->nombre << endl;
    cout << "Categoria: " << p->categoria << endl;
    cout << "Precio: Q" << p->precio << endl;
    cout << "Stock Disponible: " << p->stock << endl;
    cout << "----------------------------" << endl;
}

void buscarProductoMenu() {
    char codigoBuscado[10];
    cout << "\n--- BUSQUEDA DE PRODUCTO ---" << endl;
    cout << "Ingrese el codigo del producto a buscar: ";
    cin.getline(codigoBuscado, 10);
    Producto* productoEncontrado = buscarProducto(codigoBuscado);
    if (productoEncontrado != NULL) {
        cout << "[EXITO] Producto encontrado:" << endl;
        mostrarProducto(productoEncontrado);
    } else {
        cout << "[ERROR] Producto con codigo '" << codigoBuscado << "' no encontrado." << endl;
    }
}

void modificarProducto() {
    char codigoBuscado[10];
    cout << "\n--- MODIFICACION DE PRODUCTO ---" << endl;
    cout << "Ingrese el codigo del producto a modificar: ";
    cin.getline(codigoBuscado, 10);
    Producto* p = buscarProducto(codigoBuscado); 
    if (p == NULL) {
        cout << "[ERROR] Producto con codigo '" << codigoBuscado << "' no encontrado. No se puede modificar." << endl;
        return;
    }

    cout << "\n[EXITO] Producto encontrado. Modificando (ingrese 'm' para mantener el valor):" << endl;
    char entrada[50]; double nuevoPrecio; int nuevoStock;
    
    cout << "Ingrese NUEVO Nombre (actual: " << p->nombre << "): ";
    cin.getline(entrada, 50);
    if (strcmp(entrada, "m") != 0 && strlen(entrada) > 0) { strcpy(p->nombre, entrada); }
    
    cout << "Ingrese NUEVA Categoria (actual: " << p->categoria << "): ";
    cin.getline(entrada, 20);
    if (strcmp(entrada, "m") != 0 && strlen(entrada) > 0) { strcpy(p->categoria, entrada); }

    cout << "Ingrese NUEVO Precio (actual: Q" << p->precio << ", 'm' para mantener): ";
    cin.getline(entrada, 20);
    if (strcmp(entrada, "m") != 0 && sscanf(entrada, "%lf", &nuevoPrecio) == 1 && nuevoPrecio > 0) {
        p->precio = nuevoPrecio;
    } else if (strcmp(entrada, "m") != 0 && strlen(entrada) > 0) { cout << "[AVISO] Precio invalido. Precio no modificado." << endl; }
    
    cout << "Ingrese NUEVO Stock (actual: " << p->stock << ", 'm' para mantener): ";
    cin.getline(entrada, 20);
    if (strcmp(entrada, "m") != 0 && sscanf(entrada, "%d", &nuevoStock) == 1 && nuevoStock >= 0) {
        p->stock = nuevoStock;
    } else if (strcmp(entrada, "m") != 0 && strlen(entrada) > 0) { cout << "[AVISO] Stock invalido. Stock no modificado." << endl; }
    
    cout << "\n[EXITO] Producto con codigo '" << p->codigo << "' modificado correctamente." << endl;
    mostrarProducto(p); 
}

void eliminarProducto() {
    char codigoBuscado[10];
    cout << "\n--- ELIMINACION DE PRODUCTO ---" << endl;
    if (numProductos == 0) { cout << "[AVISO] No hay productos para eliminar." << endl; return; }
    cout << "Ingrese el codigo del producto a eliminar: ";
    cin.getline(codigoBuscado, 10);
    int indiceAEliminar = -1; 
    for (int i = 0; i < numProductos; i++) {
        if (strcmp(inventario[i].codigo, codigoBuscado) == 0) {
            indiceAEliminar = i; break;
        }
    }

    if (indiceAEliminar == -1) { cout << "[ERROR] Producto con codigo '" << codigoBuscado << "' no encontrado." << endl; return; }
    cout << "[AVISO] Producto encontrado. Eliminando: " << inventario[indiceAEliminar].nombre << endl;

    for (int i = indiceAEliminar; i < numProductos - 1; i++) {
        inventario[i] = inventario[i + 1];
    }
    numProductos--; 
    cout << "\n[EXITO] Producto '" << codigoBuscado << "' eliminado correctamente. Productos restantes: " << numProductos << endl;
}

// =========================================================================
// MODULO DE CLIENTES
// =========================================================================

void moduloClientes() {
    int opcion;
    do {
        cout << "\n--------------------------------------" << endl;
        cout << "      MODULO DE CLIENTES" << endl;
        cout << "--------------------------------------" << endl;
        cout << "1. Ingreso de Nuevo Cliente" << endl;
        cout << "2. Modificacion de Cliente" << endl;
        cout << "3. Busqueda de Cliente" << endl;
        cout << "4. Eliminacion de Cliente" << endl;
        cout << "5. Volver al Menu Principal" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cin.clear(); cin.ignore(10000, '\n'); opcion = 0;
        }
        cin.ignore(); 

        switch (opcion) {
            case 1: ingresarCliente(); break;
            case 2: modificarCliente(); break;
            case 3: buscarClienteMenu(); break;
            case 4: eliminarCliente(); break;
            case 5: cout << "\nVolviendo al Menu Principal..." << endl; break;
            default: cout << "\n[ERROR] Opcion no valida. Intente con un numero del 1 al 5." << endl;
        }
    } while (opcion != 5);
}

Cliente* buscarCliente(const char* nitBuscado) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(listaClientes[i].nit, nitBuscado) == 0) {
            return &listaClientes[i]; 
        }
    }
    return NULL; 
}

void mostrarCliente(Cliente *c) {
    cout << "\n--- DETALLE DEL CLIENTE ---" << endl;
    cout << "NIT: " << c->nit << endl;
    cout << "Nombre: " << c->nombre << endl;
    cout << "Direccion: " << c->direccion << endl;
    cout << "Telefono: " << c->telefono << endl;
    cout << "----------------------------" << endl;
}

void ingresarCliente() {
    if (numClientes >= MAX_CLIENTES) {
        cout << "\n[!] ERROR: La lista de clientes esta llena. No se puede ingresar mas clientes." << endl;
        return; 
    }

    cout << "\n--- INGRESO DE NUEVO CLIENTE ---" << endl;
    Cliente nuevoCliente; 
    
    cout << "Ingrese NIT del Cliente (max 14 caracteres): ";
    cin.getline(nuevoCliente.nit, 15); 
    
    cout << "Ingrese Nombre Completo o Razon Social (max 49 caracteres): ";
    cin.getline(nuevoCliente.nombre, 50);

    cout << "Ingrese Direccion (max 99 caracteres): ";
    cin.getline(nuevoCliente.direccion, 100);

    cout << "Ingrese Telefono (max 9 caracteres): ";
    cin.getline(nuevoCliente.telefono, 10);
    
    listaClientes[numClientes] = nuevoCliente;
    numClientes++; 

    cout << "\n[EXITO] Cliente '" << nuevoCliente.nombre << "' ingresado. Clientes totales: " << numClientes << endl;
}

void buscarClienteMenu() {
    char nitBuscado[15];

    cout << "\n--- BUSQUEDA DE CLIENTE ---" << endl;
    cout << "Ingrese el NIT del cliente a buscar: ";
    cin.getline(nitBuscado, 15);
    
    Cliente* clienteEncontrado = buscarCliente(nitBuscado);
    
    if (clienteEncontrado != NULL) {
        cout << "[EXITO] Cliente encontrado:" << endl;
        mostrarCliente(clienteEncontrado);
    } else {
        cout << "[ERROR] Cliente con NIT '" << nitBuscado << "' no encontrado." << endl;
    }
}

void modificarCliente() {
    char nitBuscado[15];
    cout << "\n--- MODIFICACION DE CLIENTE ---" << endl;
    cout << "Ingrese el NIT del cliente a modificar: ";
    cin.getline(nitBuscado, 15);

    Cliente* c = buscarCliente(nitBuscado); 
    if (c == NULL) {
        cout << "[ERROR] Cliente con NIT '" << nitBuscado << "' no encontrado. No se puede modificar." << endl;
        return;
    }

    cout << "\n[EXITO] Cliente encontrado. Modificando (ingrese 'm' para mantener el valor):" << endl;
    char entrada[100];
    
    cout << "Ingrese NUEVO Nombre (actual: " << c->nombre << "): ";
    cin.getline(entrada, 50);
    if (strcmp(entrada, "m") != 0 && strlen(entrada) > 0) { strcpy(c->nombre, entrada); }
    
    cout << "Ingrese NUEVA Direccion (actual: " << c->direccion << "): ";
    cin.getline(entrada, 100);
    if (strcmp(entrada, "m") != 0 && strlen(entrada) > 0) { strcpy(c->direccion, entrada); }

    cout << "Ingrese NUEVO Telefono (actual: " << c->telefono << "): ";
    cin.getline(entrada, 10);
    if (strcmp(entrada, "m") != 0 && strlen(entrada) > 0) { strcpy(c->telefono, entrada); }
    
    cout << "\n[EXITO] Cliente con NIT '" << c->nit << "' modificado correctamente." << endl;
    mostrarCliente(c); 
}

void eliminarCliente() {
    char nitBuscado[15];
    cout << "\n--- ELIMINACION DE CLIENTE ---" << endl;
    if (numClientes == 0) { cout << "[AVISO] No hay clientes para eliminar." << endl; return; }
    cout << "Ingrese el NIT del cliente a eliminar: ";
    cin.getline(nitBuscado, 15);
    int indiceAEliminar = -1; 
    
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(listaClientes[i].nit, nitBuscado) == 0) {
            indiceAEliminar = i; break;
        }
    }

    if (indiceAEliminar == -1) { cout << "[ERROR] Cliente con NIT '" << nitBuscado << "' no encontrado." << endl; return; }
    cout << "[AVISO] Cliente encontrado. Eliminando: " << listaClientes[indiceAEliminar].nombre << endl;

    for (int i = indiceAEliminar; i < numClientes - 1; i++) {
        listaClientes[i] = listaClientes[i + 1];
    }
    numClientes--; 
    cout << "\n[EXITO] Cliente '" << nitBuscado << "' eliminado correctamente. Clientes restantes: " << numClientes << endl;
}

// =========================================================================
// MODULO DE FACTURACION
// =========================================================================

void moduloFacturacion() {
    int opcion;

    do {
        cout << "\n--------------------------------------" << endl;
        cout << "      MODULO DE FACTURACION" << endl;
        cout << "--------------------------------------" << endl;
        cout << "1. Generar Nueva Factura" << endl;
        cout << "2. Listar Facturas" << endl;
        cout << "3. Volver al Menu Principal" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cin.clear(); cin.ignore(10000, '\n'); opcion = 0;
        }
        cin.ignore(); 

        switch (opcion) {
            case 1: generarFactura(); break;
            case 2: listarFacturasMenu(); break;
            case 3: cout << "\nVolviendo al Menu Principal..." << endl; break;
            default: cout << "\n[ERROR] Opcion no valida. Intente con un numero del 1 al 3." << endl;
        }
    } while (opcion != 3);
}

void obtenerFecha(char *buffer, size_t size) {
    time_t tiempoActual = time(NULL);
    struct tm *infoTiempo = localtime(&tiempoActual);
    strftime(buffer, size, "%Y-%m-%d", infoTiempo);
}

void generarFactura() {
    if (numFacturas >= MAX_FACTURAS) {
        cout << "[ERROR] Limite de facturas alcanzado. No se puede generar mas." << endl;
        return;
    }
    if (numClientes == 0 || numProductos == 0) {
        cout << "[AVISO] Asegurese de ingresar Clientes y Productos antes de facturar." << endl;
        return;
    }

    Factura nuevaFactura;
    Cliente *c = NULL;
    char nitBuscado[15];
    int detalleCount = 0;
    DetalleVenta *tempDetalles = NULL; 
    double subTotal = 0.0;
    const double IVA = 0.12;

    cout << "\n--- GENERAR NUEVA FACTURA #" << numFacturaActual << " ---" << endl;

    // 1. Ingreso y Busqueda del Cliente
    cout << "Ingrese NIT del Cliente: ";
    cin.getline(nitBuscado, 15);
    c = buscarCliente(nitBuscado); 
    if (c == NULL) {
        cout << "[ERROR] Cliente con NIT no encontrado. Factura cancelada." << endl;
        return;
    }
    cout << "[AVISO] Cliente: " << c->nombre << endl;
    strcpy(nuevaFactura.nitCliente, c->nit);

    // 2. Ingreso de los Productos (Estructura Repetitiva y Memoria Dinamica)
    char respuesta = 'S';
    do {
        char codProducto[10];
        int cantidad;
        Producto *p = NULL;

        cout << "\n--- DETALLE " << detalleCount + 1 << " ---" << endl;
        cout << "Ingrese Codigo de Producto: ";
        cin.getline(codProducto, 10);
        
        p = buscarProducto(codProducto); // Uso de Punteros

        if (p == NULL) {
            cout << "[AVISO] Producto no encontrado. Intente de nuevo." << endl;
            continue; 
        }

        // 3. Validacion de Stock
        cout << "Stock disponible: " << p->stock << ". Ingrese cantidad: ";
        if (!(cin >> cantidad) || cantidad <= 0 || cantidad > p->stock) {
            cout << "[ERROR] Cantidad invalida o stock insuficiente. Detalle omitido." << endl;
            cin.clear(); cin.ignore(10000, '\n');
            cout << "¿Desea agregar otro item? (S/N): ";
            cin >> respuesta;
            cin.ignore();
            continue;
        }
        cin.ignore();

        // 4. Agrandar la Memoria Dinamica (Concepto: Punteros/Memoria)
        DetalleVenta *nuevoDetalle = (DetalleVenta*)realloc(tempDetalles, (detalleCount + 1) * sizeof(DetalleVenta));
        if (nuevoDetalle == NULL) {
            cout << "[FATAL] Error de memoria. Factura cancelada." << endl;
            if (tempDetalles != NULL) free(tempDetalles);
            return;
        }
        tempDetalles = nuevoDetalle;

        // 5. Llenar el DetalleVenta
        DetalleVenta *currentDetalle = &tempDetalles[detalleCount];
        strcpy(currentDetalle->codProducto, p->codigo);
        currentDetalle->cantidad = cantidad;
        currentDetalle->precioUnitario = p->precio;
        
        subTotal += (cantidad * p->precio);
        
        // 6. Actualizar Stock del Producto ORIGINAL (Uso de Punteros)
        p->stock -= cantidad;

        detalleCount++;
        
        cout << "[AVISO] Item agregado. Subtotal actual: Q" << subTotal << endl;
        cout << "¿Desea agregar otro item? (S/N): ";
        cin >> respuesta;
        cin.ignore();

    } while (respuesta == 'S' || respuesta == 's');

    // 7. Finalizar Factura
    if (detalleCount == 0) {
        cout << "[AVISO] Factura vacia. Cancelando operacion." << endl;
        if (tempDetalles != NULL) free(tempDetalles);
        return;
    }

    double totalIVA = subTotal * IVA;
    nuevaFactura.total = subTotal + totalIVA;

    // 8. Asignar los valores finales a la Factura
    nuevaFactura.numeroFactura = numFacturaActual;
    obtenerFecha(nuevaFactura.fecha, sizeof(nuevaFactura.fecha));
    nuevaFactura.detalles = tempDetalles; // Asignamos la memoria dinamica
    nuevaFactura.numDetalles = detalleCount;

    // 9. Guardar la Factura en el Arreglo Global
    listaFacturas[numFacturas] = nuevaFactura;
    numFacturas++;
    numFacturaActual++; // Incrementamos el correlativo para la siguiente factura

    cout << "\n==========================================" << endl;
    cout << "          FACTURA GENERADA" << endl;
    cout << "==========================================" << endl;
    cout << "FACTURA # " << nuevaFactura.numeroFactura << " (" << nuevaFactura.fecha << ")" << endl;
    cout << "CLIENTE: " << c->nombre << " (NIT: " << c->nit << ")" << endl;
    cout << "SUBTOTAL: Q" << subTotal << endl;
    cout << "IVA (12%): Q" << totalIVA << endl;
    cout << "TOTAL: Q" << nuevaFactura.total << endl;
    cout << "==========================================" << endl;
}

void listarFacturasMenu() {
    cout << "\n--- LISTADO DE FACTURAS ---" << endl;
    if (numFacturas == 0) {
        cout << "[AVISO] No hay facturas registradas aun." << endl;
        return;
    }
    
    for (int i = 0; i < numFacturas; i++) {
        cout << "Factura #" << listaFacturas[i].numeroFactura 
             << " | NIT: " << listaFacturas[i].nitCliente
             << " | Fecha: " << listaFacturas[i].fecha
             << " | TOTAL: Q" << listaFacturas[i].total << endl;
    }
}

// ----------------------------------------------------------------------------------
// IMPLEMENTACIONES DE ARCHIVOS (PERSISTENCIA)
// ----------------------------------------------------------------------------------

void guardarCorrelativo() {
    ofstream archivoSalida("correlativo.txt", ios::out);
    if (archivoSalida.is_open()) {
        archivoSalida << numFacturaActual;
        archivoSalida.close();
    }
}

void cargarCorrelativo() {
    ifstream archivoEntrada("correlativo.txt", ios::in);
    if (archivoEntrada.is_open()) {
        archivoEntrada >> numFacturaActual;
        archivoEntrada.close();
    } else {
        numFacturaActual = 1; 
    }
}

void guardarDatos() {
    ofstream archivoSalida("inventario.txt", ios::out);
    if (!archivoSalida.is_open()) { cout << "[ERROR] No se pudo crear 'inventario.txt'." << endl; return; }
    archivoSalida << numProductos << endl;
    for (int i = 0; i < numProductos; i++) {
        archivoSalida << inventario[i].codigo << "|"
                      << inventario[i].nombre << "|"
                      << inventario[i].categoria << "|"
                      << inventario[i].precio << "|"
                      << inventario[i].stock << endl; 
    }
    archivoSalida.close();
    cout << "\n[EXITO] Inventario guardado correctamente en 'inventario.txt'." << endl;
}

void cargarDatos() {
    ifstream archivoEntrada("inventario.txt", ios::in);
    if (!archivoEntrada.is_open()) { numProductos = 0; return; }
    archivoEntrada >> numProductos;
    archivoEntrada.ignore();
    char linea[200]; 
    for (int i = 0; i < numProductos; i++) {
        if (archivoEntrada.getline(linea, 200)) {
            char *token;
            token = strtok(linea, "|");
            if (token != NULL) strcpy(inventario[i].codigo, token);
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(inventario[i].nombre, token);
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(inventario[i].categoria, token);
            token = strtok(NULL, "|");
            if (token != NULL) inventario[i].precio = atof(token); 
            token = strtok(NULL, "|");
            if (token != NULL) inventario[i].stock = atoi(token); 
        }
    }
    archivoEntrada.close();
    cout << "\n[EXITO] Se cargaron " << numProductos << " productos desde 'inventario.txt'." << endl;
}

void guardarClientes() {
    ofstream archivoSalida("clientes.txt", ios::out);
    if (!archivoSalida.is_open()) { cout << "[ERROR] No se pudo crear 'clientes.txt'." << endl; return; }
    archivoSalida << numClientes << endl;
    for (int i = 0; i < numClientes; i++) {
        archivoSalida << listaClientes[i].nit << "|"
                      << listaClientes[i].nombre << "|"
                      << listaClientes[i].direccion << "|"
                      << listaClientes[i].telefono << endl; 
    }
    archivoSalida.close();
    cout << "\n[EXITO] Clientes guardados correctamente en 'clientes.txt'." << endl;
}

void cargarClientes() {
    ifstream archivoEntrada("clientes.txt", ios::in);
    if (!archivoEntrada.is_open()) { numClientes = 0; return; }
    archivoEntrada >> numClientes;
    archivoEntrada.ignore(); 
    char linea[200]; 
    for (int i = 0; i < numClientes; i++) {
        if (archivoEntrada.getline(linea, 200)) {
            char *token;
            token = strtok(linea, "|");
            if (token != NULL) strcpy(listaClientes[i].nit, token);
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(listaClientes[i].nombre, token);
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(listaClientes[i].direccion, token);
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(listaClientes[i].telefono, token);
        }
    }
    archivoEntrada.close();
    cout << "\n[EXITO] Se cargaron " << numClientes << " clientes desde 'clientes.txt'." << endl;
}

// Guarda los datos de las facturas (Memoria dinamica)
void guardarFacturas() {
    ofstream archivoSalida("facturas.txt", ios::out);
    if (!archivoSalida.is_open()) { cout << "[ERROR] No se pudo crear 'facturas.txt'." << endl; return; }
    
    // 1. Escribir el contador principal (cuantas facturas existen)
    archivoSalida << numFacturas << endl; 
    
    // 2. Iterar sobre cada factura (Estructura Repetitiva)
    for (int i = 0; i < numFacturas; i++) {
        Factura f = listaFacturas[i];
        
        // Escribir cabecera de la factura, incluyendo el numDetalles
        archivoSalida << f.numeroFactura << "|"
                      << f.nitCliente << "|"
                      << f.fecha << "|"
                      << f.total << "|"
                      << f.numDetalles << endl; // CLAVE: Guardamos cuantos detalles tiene
                      
        // 3. Escribir cada detalle de esa factura (Bucle anidado - Estructura Repetitiva)
        for (int j = 0; j < f.numDetalles; j++) {
            archivoSalida << f.detalles[j].codProducto << "|"
                          << f.detalles[j].cantidad << "|"
                          << f.detalles[j].precioUnitario << endl;
        }
    }
    archivoSalida.close();
    cout << "\n[EXITO] Facturas guardadas correctamente en 'facturas.txt'." << endl;
}

// Carga los datos de las facturas (Memoria dinamica)
void cargarFacturas() {
    ifstream archivoEntrada("facturas.txt", ios::in);
    if (!archivoEntrada.is_open()) { numFacturas = 0; return; }
    
    archivoEntrada >> numFacturas;
    archivoEntrada.ignore();
    
    char linea[200];
    
    // Estructura Repetitiva: Iteramos sobre el numero total de facturas
    for (int i = 0; i < numFacturas; i++) {
        // 1. Leer la cabecera de la factura
        if (archivoEntrada.getline(linea, 200)) {
            char *token;
            Factura *f = &listaFacturas[i]; // Puntero al lugar donde guardaremos la factura

            token = strtok(linea, "|"); if (token != NULL) f->numeroFactura = atoi(token);
            token = strtok(NULL, "|"); if (token != NULL) strcpy(f->nitCliente, token);
            token = strtok(NULL, "|"); if (token != NULL) strcpy(f->fecha, token);
            token = strtok(NULL, "|"); if (token != NULL) f->total = atof(token);
            token = strtok(NULL, "|"); if (token != NULL) f->numDetalles = atoi(token); // Leemos cuantos detalles hay
            
            // 2. Crear el arreglo de Detalles (Memoria Dinamica)
            // Cuidado: realloc no se usa para crear el arreglo por primera vez, solo malloc/calloc
            f->detalles = (DetalleVenta*)malloc(f->numDetalles * sizeof(DetalleVenta));
            if (f->detalles == NULL && f->numDetalles > 0) { 
                cout << "[ERROR FATAL] Error al asignar memoria para detalles de factura." << endl; 
                exit(1); 
            }

            // 3. Leer cada detalle (Bucle anidado - Estructura Repetitiva)
            for (int j = 0; j < f->numDetalles; j++) {
                if (archivoEntrada.getline(linea, 200)) {
                    token = strtok(linea, "|"); if (token != NULL) strcpy(f->detalles[j].codProducto, token);
                    token = strtok(NULL, "|"); if (token != NULL) f->detalles[j].cantidad = atoi(token);
                    token = strtok(NULL, "|"); if (token != NULL) f->detalles[j].precioUnitario = atof(token);
                }
            }
        }
    }
    archivoEntrada.close();
    cout << "\n[EXITO] Se cargaron " << numFacturas << " facturas desde 'facturas.txt'." << endl;
}

