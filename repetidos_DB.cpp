#include <iostream>
#include <sqlite3.h>   // Biblioteca SQLite

// Función callback para SELECT (imprime resultados)
static int callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " 
                  << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << "-----------------------------\n";
    return 0;
}

int main() {
    sqlite3* db;
    char* errmsg = 0;
    int rc;

    // Abrir/crear BD
    rc = sqlite3_open("mi_db.db", &db);
    if (rc) {
        std::cerr << "Error abriendo BD: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Crear tabla
    const char* sqlCreate =
        "CREATE TABLE IF NOT EXISTS Libros ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Titulo TEXT NOT NULL, "
        "Autor TEXT, "
        "Anio INTEGER);";

    rc = sqlite3_exec(db, sqlCreate, callback, 0, &errmsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error SQL: " << errmsg << std::endl;
        sqlite3_free(errmsg);
    }

    // Insertar datos
    const char* sqlInsert =
        "INSERT INTO Libros (Titulo, Autor, Anio) VALUES "
        "('1984', 'George Orwell', 1949),"
        "('Cien Años de Soledad', 'Gabriel García Márquez', 1967);";

    rc = sqlite3_exec(db, sqlInsert, callback, 0, &errmsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error INSERT: " << errmsg << std::endl;
        sqlite3_free(errmsg);
    }

    // Consultar todos los registros
    std::cout << "=== Libros en la BD ===\n";
    const char* sqlSelect = "SELECT * FROM Libros;";
    rc = sqlite3_exec(db, sqlSelect, callback, 0, &errmsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error SELECT: " << errmsg << std::endl;
        sqlite3_free(errmsg);
    }

    sqlite3_close(db);
    return 0;
}