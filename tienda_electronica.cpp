#include <iostream>
#include <cstring>  // Para usar funciones de cadenas (strcpy, strcmp, strlen)
#include <fstream>  // Para el manejo de Archivos (Criterio 6)
#include <locale>   // Necesario para manejar la configuracion regional (tildes, ñ)
#include <cstdio>   // Necesario para usar sscanf (util en modificacion)
#include <cstdlib>  // Necesario para usar atof y atoi (conversion de cadena a numero)

using namespace std;

// --- DEFINICION DE ESTRUCTURAS DE DATOS (REGISTROS) ---
// (Concepto: Registros)

// Maximo de elementos que pueden contener los arreglos (limite del sistema)
const int MAX_PRODUCTOS = 100;
const int MAX_CLIENTES = 50;

// Estructura para almacenar los detalles de un producto en el inventario.
struct Producto {
    char codigo[10];    // Clave de busqueda unica. (Concepto: Cadenas)
    char nombre[50];    // Nombre del producto
    double precio;      // Precio de venta
    int stock;          // Cantidad en inventario
    char categoria[20]; // Ejemplo: 'Laptop', 'Smartphone', 'Accesorio'
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
    DetalleVenta *detalles; // PUNTERO al arreglo de detalles (Concepto: Punteros)
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

Producto inventario[MAX_PRODUCTOS];  // Arreglo para almacenar todos los productos
int numProductos = 0;                // Contador de la cantidad actual de productos

Cliente listaClientes[MAX_CLIENTES]; // Arreglo para almacenar todos los clientes
int numClientes = 0;                 // Contador de la cantidad actual de clientes

// --- DECLARACION DE PROTOTIPOS DE FUNCIONES (Concepto: Modularizacion) ---
void menuPrincipal();
void moduloProductos();
void ingresarProducto();
void buscarProductoMenu();
Producto* buscarProducto(const char* codigoBuscado); 
void mostrarProducto(Producto *p);
void modificarProducto();
void eliminarProducto();
void guardarDatos();   
void cargarDatos();    // <--- Implementacion finalizada
void moduloClientes();


// La funcion principal del programa
int main() {
    // ----------------------------------------------------------------------
    // SOLUCION PARA CARACTERES ESPECIALES (TILDES, Ñ) EN LA CONSOLA:
    setlocale(LC_ALL, ""); 
    // ----------------------------------------------------------------------
    
    // Al iniciar el programa, cargamos los datos guardados de la sesion anterior.
    cargarDatos(); // <--- Llamada a la funcion de carga
    
    menuPrincipal(); // Iniciar el menu
    return 0;
}

// Implementacion del Menu Principal
void menuPrincipal() {
    int opcion; // Variable para la estructura selectiva

    // Estructura Repetitiva do-while: Se repite hasta que el usuario elija Salir (opcion != 6)
    do {
        // system("cls") o system("clear") se usa para limpiar la pantalla (depende del OS)
        cout << "\n======================================" << endl;
        cout << "     SISTEMA TIENDA DE ELECTRONICOS" << endl;
        cout << "======================================" << endl;
        cout << "1. Modulo de Productos" << endl;
        cout << "2. Modulo de Clientes" << endl;
        cout << "3. Modulo de Ventas / Facturacion" << endl;
        cout << "4. Modulo de Reportes" << endl;
        cout << "5. Modulo de Archivos (Guardar y Cargar)" << endl;
        cout << "6. Salir del Sistema" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        
        // Manejo de entrada para evitar errores si el usuario ingresa texto
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = 0; // Opcion invalida
        } else {
             cin.ignore(); // Limpiamos el buffer despues de leer el numero
        }

        // Estructura Selectiva switch: Evalua la opcion elegida para dirigir el flujo
        switch (opcion) {
            case 1:
                moduloProductos();
                break;
            case 2:
                // moduloClientes();
                cout << "\n[!] Entrando al Modulo de Clientes (Pendiente de implementar)..." << endl;
                break;
            case 3:
                cout << "\n[!] Entrando al Modulo de Ventas / Facturacion (Pendiente de implementar)..." << endl;
                break;
            case 4:
                cout << "\n[!] Entrando al Modulo de Reportes (Pendiente de implementar)..." << endl;
                break;
            case 5:
                guardarDatos(); 
                // Nota: La funcion cargarDatos ya se llama al inicio en main()
                break;
            case 6:
                cout << "\nSaliendo del sistema. ¡Hasta pronto!" << endl;
                break;
            default:
                cout << "\n[ERROR] Opcion no valida. Por favor, intente de nuevo." << endl;
        }

    } while (opcion != 6);
}

// Implementacion del Modulo de Productos (Submenu)
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
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            opcion = 0;
        }
        
        cin.ignore(); // Limpiamos el buffer

        switch (opcion) {
            case 1:
                ingresarProducto(); 
                break;
            case 2:
                modificarProducto();
                break;
            case 3:
                buscarProductoMenu();
                break;
            case 4:
                eliminarProducto();
                break;
            case 5:
                cout << "\nVolviendo al Menu Principal..." << endl;
                break;
            default:
                cout << "\n[ERROR] Opcion no valida. Intente con un numero del 1 al 5." << endl;
        }

    } while (opcion != 5);
}

// Implementacion de la funcion para ingresar un nuevo producto (CORREGIDO el problema de buffer)
void ingresarProducto() {
    // Estructura Selectiva: Verificamos si el arreglo esta lleno
    if (numProductos >= MAX_PRODUCTOS) {
        cout << "\n[!] ERROR: El inventario esta lleno. No se puede ingresar mas productos." << endl;
        return; 
    }

    cout << "\n--- INGRESO DE NUEVO PRODUCTO ---" << endl;

    Producto nuevoProducto; 
    
    // Ingreso y validacion del codigo (debe ser unico, pero por ahora solo leemos)
    cout << "Ingrese Codigo (max 9 caracteres): ";
    cin.getline(nuevoProducto.codigo, 10); 
    
    // Ingreso de Nombre (Concepto: Cadenas)
    cout << "Ingrese Nombre del Producto (max 49 caracteres): ";
    cin.getline(nuevoProducto.nombre, 50);

    // Ingreso de Categoria (Concepto: Cadenas)
    cout << "Ingrese Categoria (Ej: Laptop, Accesorio): ";
    cin.getline(nuevoProducto.categoria, 20);

    // Ingreso de Precio (Validacion de tipo de dato - Estructura Repetitiva)
    double tempPrecio; // Variable temporal para la validacion (ayuda a evitar Segmentation Fault)
    cout << "Ingrese Precio: ";
    while (!(cin >> tempPrecio) || tempPrecio <= 0) {
        cout << "[ERROR] Entrada invalida. Ingrese un precio numerico positivo: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    nuevoProducto.precio = tempPrecio;

    // Ingreso de Stock (Validacion de tipo de dato - Estructura Repetitiva)
    int tempStock; // Variable temporal para la validacion
    cout << "Ingrese Stock Inicial: ";
    while (!(cin >> tempStock) || tempStock < 0) {
        cout << "[ERROR] Entrada invalida. Ingrese un stock entero no negativo: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    nuevoProducto.stock = tempStock;
    
    cin.ignore(); // Limpiamos el buffer despues de la ULTIMA lectura numerica
    
    // Asignacion: Copiamos la data de la variable local al arreglo global 'inventario'
    inventario[numProductos] = nuevoProducto;
    
    // Control: Aumentamos el contador global, apuntando al siguiente espacio disponible
    numProductos++; 

    cout << "\n[EXITO] Producto '" << nuevoProducto.nombre << "' ingresado. Productos totales: " << numProductos << endl;
}


// --- IMPLEMENTACION DE BUSQUEDA (Concepto: Punteros) ---

// Funcion Logica de Busqueda. Retorna la DIRECCION del producto o NULL.
Producto* buscarProducto(const char* codigoBuscado) {
    // Estructura Repetitiva for: Itera sobre todos los productos que existen (numProductos)
    for (int i = 0; i < numProductos; i++) {
        // Estructura Selectiva if: Compara el codigo buscado con el codigo del producto actual
        if (strcmp(inventario[i].codigo, codigoBuscado) == 0) {
            // RETORNA PUNTERO: Devolvemos la DIRECCION de memoria (&) de la estructura Producto en el indice 'i'.
            return &inventario[i];
        }
    }
    // Si no lo encuentra despues de revisar todo el arreglo, retorna NULL
    return NULL; 
}

// Procedimiento Auxiliar para mostrar los detalles de un producto (pasado por puntero)
void mostrarProducto(Producto *p) {
    // Usamos el operador flecha (->) para acceder a los miembros del struct a traves del puntero 'p'
    cout << "\n--- DETALLE DEL PRODUCTO ---" << endl;
    cout << "Codigo: " << p->codigo << endl;
    cout << "Nombre: " << p->nombre << endl;
    cout << "Categoria: " << p->categoria << endl;
    cout << "Precio: Q" << p->precio << endl;
    cout << "Stock Disponible: " << p->stock << endl;
    cout << "----------------------------" << endl;
}

// Funcion de Interfaz de Busqueda (para el Menu)
void buscarProductoMenu() {
    char codigoBuscado[10];

    cout << "\n--- BUSQUEDA DE PRODUCTO ---" << endl;
    cout << "Ingrese el codigo del producto a buscar: ";
    cin.getline(codigoBuscado, 10);
    
    // Llamamos a la funcion de busqueda que nos devuelve un PUNTERO
    Producto* productoEncontrado = buscarProducto(codigoBuscado);
    
    // Estructura Selectiva: Evaluamos el puntero (NULL significa "no encontrado")
    if (productoEncontrado != NULL) {
        cout << "[EXITO] Producto encontrado:" << endl;
        // Le pasamos el puntero a la funcion que lo muestra
        mostrarProducto(productoEncontrado);
    } else {
        cout << "[ERROR] Producto con codigo '" << codigoBuscado << "' no encontrado." << endl;
    }
}

// Implementacion de la Modificacion de Producto (Uso de Punteros para acceso directo)
void modificarProducto() {
    char codigoBuscado[10];

    cout << "\n--- MODIFICACION DE PRODUCTO ---" << endl;
    cout << "Ingrese el codigo del producto a modificar: ";
    cin.getline(codigoBuscado, 10);

    // 1. Busqueda (Uso del Puntero)
    Producto* p = buscarProducto(codigoBuscado); 

    // 2. Estructura Selectiva (Verificar si el puntero es valido)
    if (p == NULL) {
        cout << "[ERROR] Producto con codigo '" << codigoBuscado << "' no encontrado. No se puede modificar." << endl;
        return;
    }

    // 3. Producto encontrado: Lo mostramos para confirmar
    cout << "\n[EXITO] Producto encontrado. Modificando (ingrese 'm' para mantener el valor):" << endl;
    
    char entrada[50];
    double nuevoPrecio;
    int nuevoStock;
    
    // Modificar Nombre
    cout << "Ingrese NUEVO Nombre (actual: " << p->nombre << "): ";
    cin.getline(entrada, 50);
    if (strcmp(entrada, "m") != 0 && strlen(entrada) > 0) {
        strcpy(p->nombre, entrada);
    }
    
    // Modificar Categoria
    cout << "Ingrese NUEVA Categoria (actual: " << p->categoria << "): ";
    cin.getline(entrada, 20);
    if (strcmp(entrada, "m") != 0 && strlen(entrada) > 0) {
        strcpy(p->categoria, entrada);
    }

    // Modificar Precio (Validacion con Estructura Repetitiva)
    cout << "Ingrese NUEVO Precio (actual: Q" << p->precio << ", 'm' para mantener): ";
    cin.getline(entrada, 20);
    if (strcmp(entrada, "m") != 0) {
        // sscanf intenta leer un double (%lf) de la cadena 'entrada' y lo guarda en &nuevoPrecio
        if (sscanf(entrada, "%lf", &nuevoPrecio) == 1 && nuevoPrecio > 0) {
             p->precio = nuevoPrecio;
        } else {
            cout << "[AVISO] Valor de precio invalido o no numerico. Precio no modificado." << endl;
        }
    }
    
    // Modificar Stock (Validacion con Estructura Repetitiva)
    cout << "Ingrese NUEVO Stock (actual: " << p->stock << ", 'm' para mantener): ";
    cin.getline(entrada, 20);
    if (strcmp(entrada, "m") != 0) {
        // sscanf intenta leer un entero (%d) de la cadena 'entrada' y lo guarda en &nuevoStock
        if (sscanf(entrada, "%d", &nuevoStock) == 1 && nuevoStock >= 0) {
             p->stock = nuevoStock;
        } else {
            cout << "[AVISO] Valor de stock invalido o no entero. Stock no modificado." << endl;
        }
    }
    
    cout << "\n[EXITO] Producto con codigo '" << p->codigo << "' modificado correctamente." << endl;
    mostrarProducto(p); // Mostrar el resultado final
}

// Implementacion de la Eliminacion de Producto (Algoritmo de Arreglos)
void eliminarProducto() {
    char codigoBuscado[10];

    cout << "\n--- ELIMINACION DE PRODUCTO ---" << endl;
    if (numProductos == 0) {
        cout << "[AVISO] No hay productos para eliminar." << endl;
        return;
    }

    cout << "Ingrese el codigo del producto a eliminar: ";
    cin.getline(codigoBuscado, 10);

    int indiceAEliminar = -1; // Usaremos un indice en lugar de un puntero para la eliminacion
    
    // 1. Busqueda por indice (Estructura Repetitiva)
    for (int i = 0; i < numProductos; i++) {
        if (strcmp(inventario[i].codigo, codigoBuscado) == 0) {
            indiceAEliminar = i;
            break;
        }
    }

    // 2. Estructura Selectiva: Evaluar si se encontro
    if (indiceAEliminar == -1) {
        cout << "[ERROR] Producto con codigo '" << codigoBuscado << "' no encontrado." << endl;
        return;
    }
    
    // 3. Proceso de Eliminacion y Corrimiento del Arreglo (Concepto: Arreglos)
    cout << "[AVISO] Producto encontrado. Eliminando: " << inventario[indiceAEliminar].nombre << endl;

    // Estructura Repetitiva for: Correr los elementos una posicion hacia la izquierda
    // Esto sobrescribe el elemento a eliminar.
    for (int i = indiceAEliminar; i < numProductos - 1; i++) {
        inventario[i] = inventario[i + 1];
    }
    
    // 4. Actualizar el Contador
    numProductos--; // Reducimos el contador para ignorar el ultimo elemento duplicado

    cout << "\n[EXITO] Producto '" << codigoBuscado << "' eliminado correctamente. Productos restantes: " << numProductos << endl;
}

// Implementacion para guardar todos los datos del inventario en un archivo de texto
// (Concepto: Archivos / Criterio 6)
void guardarDatos() {
    // ofstream significa "Output File Stream" (Flujo de Salida de Archivo)
    // Abrimos el archivo llamado "inventario.txt". ios::out indica que es para escritura.
    ofstream archivoSalida("inventario.txt", ios::out);

    // Estructura Selectiva: Verificamos si el archivo se abrio correctamente
    if (!archivoSalida.is_open()) {
        cout << "[ERROR] No se pudo abrir o crear el archivo 'inventario.txt'. Verifique permisos de la carpeta." << endl;
        return;
    }

    // Escribimos primero la cantidad de productos total (numProductos)
    // Esto es CLAVE para que la funcion 'cargarDatos' sepa cuantos productos leer.
    archivoSalida << numProductos << endl;

    // Estructura Repetitiva for: Recorremos todo el arreglo de productos
    for (int i = 0; i < numProductos; i++) {
        // Escribimos cada campo del struct Producto, separado por un pipe '|'
        archivoSalida << inventario[i].codigo << "|"
                      << inventario[i].nombre << "|"
                      << inventario[i].categoria << "|"
                      << inventario[i].precio << "|"
                      << inventario[i].stock << endl; // Usamos 'endl' para el salto de linea
    }

    // Cerramos el archivo para liberar los recursos
    archivoSalida.close();
    cout << "\n[EXITO] Inventario guardado correctamente en 'inventario.txt'." << endl;
}

// Implementacion para cargar los datos del inventario desde el archivo de texto
// (Concepto: Archivos / Criterio 6)
void cargarDatos() {
    // ifstream significa "Input File Stream" (Flujo de Entrada de Archivo)
    ifstream archivoEntrada("inventario.txt", ios::in);
    
    // Estructura Selectiva 1: Verificamos si el archivo existe
    if (!archivoEntrada.is_open()) {
        cout << "[AVISO] Archivo 'inventario.txt' no encontrado. Iniciando inventario vacio." << endl;
        numProductos = 0; // Aseguramos que el contador este en cero si no hay archivo.
        return;
    }

    // 1. Leer el contador global (numProductos)
    // Leemos la primera linea para saber cuantos productos hay guardados.
    archivoEntrada >> numProductos;
    archivoEntrada.ignore(); // Limpiar el buffer despues de leer el numero

    // Variables temporales para la lectura de la linea completa y los campos
    char linea[200]; 
    
    // Estructura Repetitiva for: Iteramos solo la cantidad de productos que se nos indica en la primera linea
    for (int i = 0; i < numProductos; i++) {
        // Leemos la linea completa del producto
        if (archivoEntrada.getline(linea, 200)) {
            
            // Usaremos un buffer temporal para la lectura de los campos
            // Usamos strtok para dividir la linea por el delimitador '|'
            char *token;
            
            // Primer campo: Codigo
            token = strtok(linea, "|");
            if (token != NULL) strcpy(inventario[i].codigo, token);
            
            // Segundo campo: Nombre
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(inventario[i].nombre, token);
            
            // Tercer campo: Categoria
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(inventario[i].categoria, token);
            
            // Cuarto campo: Precio
            token = strtok(NULL, "|");
            if (token != NULL) inventario[i].precio = atof(token); // atof convierte cadena a double
            
            // Quinto campo: Stock
            token = strtok(NULL, "|");
            if (token != NULL) inventario[i].stock = atoi(token); // atoi convierte cadena a int
        }
    }

    // Cerramos el archivo para liberar los recursos
    archivoEntrada.close();
    cout << "\n[EXITO] Se cargaron " << numProductos << " productos desde 'inventario.txt'." << endl;
}

