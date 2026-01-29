/*TAREA: Gestión de Biblioteca Escolar
Reestructurado para cumplir con el uso de Matrices Bidimensionales
y operaciones avanzadas.*/
//Herman Espinosa

#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Para sort, find
#include <iomanip>   // Para formato de salida

using namespace std;

//FUNCIONES DE UTILIDAD

// Función para validar entrada de números enteros
int leerEntero(string mensaje) {
    int valor;
    cout << mensaje;
    while (!(cin >> valor) || valor < 0) {
        cout << ">> Error: Entrada invalida. Ingrese un numero entero positivo: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(); // Limpiar buffer después de leer entero para evitar errores con getline
    return valor;
}

// Función para leer texto (nombres, categorías)
string leerTexto(string mensaje) {
    string texto;
    cout << mensaje;
    getline(cin, texto);
    return texto;
}

//OPERACIONES OBLIGATORIAS

// 1. y 2. Guardar e Insertar libros
void insertarLibro(vector<vector<string>>& libros, vector<vector<int>>& stock, vector<vector<int>>& prestamos) {
    cout << "\n REGISTRAR NUEVO LIBRO " << endl;
    string nombre = leerTexto("Nombre del libro: ");
    string categoria = leerTexto("Categoria: ");
    int cantidad = leerEntero("Stock inicial: ");

    // Matriz 1: Libros [Nombre, Categoria]
    vector<string> nuevoLibro = {nombre, categoria};
    // Usamos .insert() al final (equivalente a push_back pero cumpliendo el requisito)
    libros.insert(libros.end(), nuevoLibro);

    // Matriz 2: Stock [Stock Total, Prestados Activos]
    vector<int> nuevoStock = {cantidad, 0};
    stock.insert(stock.end(), nuevoStock);

    // Matriz 3: Prestamos Semanales [L, M, M, J, V] (Inicializados en 0)
    vector<int> nuevosPrestamos(5, 0); 
    prestamos.insert(prestamos.end(), nuevosPrestamos);

    cout << ">> Libro registrado con exito. ID asignado: " << libros.size() - 1 << endl;
}

// 4. Mostrar libros disponibles
void mostrarDisponibles(const vector<vector<string>>& libros, const vector<vector<int>>& stock) {
    cout << "\n LIBROS DISPONIBLES (Stock > 0) " << endl;
    cout << left << setw(5) << "ID" << setw(30) << "Nombre" << setw(20) << "Categoria" << setw(10) << "Stock" << endl;
    cout << "----------------------------------------------------------------" << endl;
    
    bool hayLibros = false;
    for (size_t i = 0; i < libros.size(); i++) {
        // stock[i][0] es el Stock Total
        if (stock[i][0] > 0) {
            cout << left << setw(5) << i 
                 << setw(30) << libros[i][0] 
                 << setw(20) << libros[i][1] 
                 << setw(10) << stock[i][0] << endl;
            hayLibros = true;
        }
    }
    if (!hayLibros) cout << ">> No hay libros con stock disponible." << endl;
}

// 3. Actualizar stock después de un préstamo
void realizarPrestamo(vector<vector<string>>& libros, vector<vector<int>>& stock, vector<vector<int>>& prestamos) {
    int id = leerEntero("Ingrese ID del libro a prestar: ");

    if (id >= 0 && id < libros.size()) {
        if (stock[id][0] > 0) {
            // Seleccionar día de la semana (0=Lunes, 4=Viernes)
            int dia = leerEntero("Dia de la semana (0=Lun, 1=Mar, 2=Mie, 3=Jue, 4=Vie): ");
            if (dia >= 0 && dia <= 4) {
                // Lógica de negocio
                stock[id][0]--; // Reducir Stock
                stock[id][1]++; // Aumentar Prestados Activos
                prestamos[id][dia]++; // Registrar en la matriz de la semana
                cout << ">> Prestamo registrado exitosamente." << endl;
            } else {
                cout << ">> Error: Dia invalido." << endl;
            }
        } else {
            cout << ">> Error: No hay stock suficiente para este libro." << endl;
        }
    } else {
        cout << ">> Error: ID no encontrado." << endl;
    }
}

// 5. Modificar información
void modificarLibro(vector<vector<string>>& libros, vector<vector<int>>& stock) {
    int id = leerEntero("Ingrese ID del libro a modificar: ");
    if (id >= 0 && id < libros.size()) {
        cout << "Modificando: " << libros[id][0] << endl;
        cout << "1. Nombre\n2. Categoria\n3. Stock\n";
        int op = leerEntero("Seleccione que modificar: ");
        
        if (op == 1) libros[id][0] = leerTexto("Nuevo nombre: ");
        else if (op == 2) libros[id][1] = leerTexto("Nueva categoria: ");
        else if (op == 3) stock[id][0] = leerEntero("Nuevo stock: ");
        else cout << ">> Opcion invalida." << endl;
    } else {
        cout << ">> Error: ID no encontrado." << endl;
    }
}

// 6. Eliminar libro (Usando erase y manteniendo sincronía de matrices)
void eliminarLibro(vector<vector<string>>& libros, vector<vector<int>>& stock, vector<vector<int>>& prestamos) {
    int id = leerEntero("Ingrese ID del libro a eliminar: ");
    if (id >= 0 && id < libros.size()) {
        // Debemos eliminar la fila 'id' de LAS TRES matrices para mantener el orden
        libros.erase(libros.begin() + id);
        stock.erase(stock.begin() + id);
        prestamos.erase(prestamos.begin() + id);
        cout << ">> Libro eliminado correctamente." << endl;
    } else {
        cout << ">> Error: ID no encontrado." << endl;
    }
}

// 7. Buscar libro (Por nombre o categoría)
void buscarLibro(const vector<vector<string>>& libros, const vector<vector<int>>& stock) {
    string consulta = leerTexto("Ingrese nombre o categoria a buscar: ");
    bool encontrado = false;
    
    cout << "\n--- RESULTADOS DE BUSQUEDA ---" << endl;
    for (size_t i = 0; i < libros.size(); i++) {
        // Búsqueda simple de subcadena (manual, ya que find busca elementos exactos en contenedores)
        // O usando find exacto como pide la tarea:
        if (libros[i][0] == consulta || libros[i][1] == consulta) {
            cout << "ID: " << i << " | " << libros[i][0] << " (" << libros[i][1] << ") | Stock: " << stock[i][0] << endl;
            encontrado = true;
        }
    }
    if (!encontrado) cout << ">> No se encontraron coincidencias exactas." << endl;
}

// 8. Ordenar libros (Burbuja para mantener sincronía entre matrices paralelas)
void ordenarLibros(vector<vector<string>>& libros, vector<vector<int>>& stock, vector<vector<int>>& prestamos) {
    cout << "Ordenar por: 1. Nombre, 2. Stock" << endl;
    int criterio = leerEntero("Seleccion: ");
    
    // Usamos algoritmo de burbuja porque necesitamos intercambiar filas en 3 matrices simultáneamente
    for (size_t i = 0; i < libros.size() - 1; i++) {
        for (size_t j = 0; j < libros.size() - i - 1; j++) {
            bool intercambiar = false;
            if (criterio == 1) { // Por Nombre
                if (libros[j][0] > libros[j + 1][0]) intercambiar = true;
            } else if (criterio == 2) { // Por Stock
                if (stock[j][0] < stock[j + 1][0]) intercambiar = true; // Descendente
            }

            if (intercambiar) {
                swap(libros[j], libros[j + 1]);
                swap(stock[j], stock[j + 1]);
                swap(prestamos[j], prestamos[j + 1]);
            }
        }
    }
    cout << ">> Libros ordenados." << endl;
}

//OPERACIONES AVANZADAS

// Reporte Avanzado: Totales, Maximos y Rotación
void reporteAvanzado(const vector<vector<string>>& libros, const vector<vector<int>>& stock, const vector<vector<int>>& prestamos) {
    if (libros.empty()) {
        cout << "No hay datos para generar reporte." << endl;
        return;
    }

    int maxPrestamos = -1;
    string libroTop = "";
    
    vector<int> diasTotales(5, 0); // Para sumar préstamos por día de la semana globalmente
    
    cout << "\n=== REPORTE DE INVENTARIO Y ESTADISTICAS ===" << endl;
    cout << left << setw(30) << "Libro" << setw(15) << "Total Semanal" << setw(15) << "Estado" << endl;

    for (size_t i = 0; i < libros.size(); i++) {
        int sumaFila = 0;
        for (int d = 0; d < 5; d++) {
            sumaFila += prestamos[i][d];
            diasTotales[d] += prestamos[i][d]; // Acumulador por día
        }

        // Determinar estado de rotación
        string estado = (sumaFila > 5) ? "Alta Rotacion" : "Normal";
        if (stock[i][0] == 0) estado = "AGOTADO";

        cout << left << setw(30) << libros[i][0] << setw(15) << sumaFila << setw(15) << estado << endl;

        // Buscar el libro más prestado
        if (sumaFila > maxPrestamos) {
            maxPrestamos = sumaFila;
            libroTop = libros[i][0];
        }
    }

    // Día más activo
    int diaMax = -1; 
    int maxDiaVal = -1;
    string nombresDias[] = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes"};
    
    for(int d=0; d<5; d++){
        if(diasTotales[d] > maxDiaVal){
            maxDiaVal = diasTotales[d];
            diaMax = d;
        }
    }

    cout << "\n[Estadisticas Generales]" << endl;
    cout << "* Libro mas popular: " << libroTop << " (" << maxPrestamos << " prestamos)" << endl;
    cout << "* Dia mas activo: " << nombresDias[diaMax] << " (Total: " << maxDiaVal << ")" << endl;
}

//MAIN

int main() {
    // Inicialización de Datos (Matrices)
    // Matriz 1: [Nombre, Categoria]
    vector<vector<string>> libros = {
        {"Don Quijote", "Literatura"},
        {"Calculo de Stewart", "Matematicas"},
        {"Clean Code", "Programacion"}
    };

    // Matriz 2: [Stock Total, Prestados Activos]
    vector<vector<int>> stock = {
        {10, 2},
        {5, 1},
        {3, 5}
    };

    // Matriz 3: [Lun, Mar, Mie, Jue, Vie]
    vector<vector<int>> prestamos = {
        {1, 0, 0, 1, 0},
        {0, 1, 0, 0, 0},
        {1, 1, 1, 1, 1}
    };

    int opcion;
    do {
        cout << "\n GESTION DE BIBLIOTECA " << endl;
        cout << "1. Mostrar Libros Disponibles" << endl;
        cout << "2. Registrar Prestamo" << endl;
        cout << "3. Agregar Nuevo Libro" << endl;
        cout << "4. Modificar Libro" << endl;
        cout << "5. Eliminar Libro" << endl;
        cout << "6. Buscar Libro" << endl;
        cout << "7. Ordenar Libros" << endl;
        cout << "8. Reporte Avanzado (Estadisticas)" << endl;
        cout << "0. Salir" << endl;
        
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: mostrarDisponibles(libros, stock); break;
            case 2: realizarPrestamo(libros, stock, prestamos); break;
            case 3: insertarLibro(libros, stock, prestamos); break;
            case 4: modificarLibro(libros, stock); break;
            case 5: eliminarLibro(libros, stock, prestamos); break;
            case 6: buscarLibro(libros, stock); break;
            case 7: ordenarLibros(libros, stock, prestamos); break;
            case 8: reporteAvanzado(libros, stock, prestamos); break;
            case 0: cout << "Saliendo del sistema..." << endl; break;
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);

    return 0;
}