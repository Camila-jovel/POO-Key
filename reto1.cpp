#include <iostream>
#include <string>
#include <vector>
#include <fstream> //para trabajar con archivos (leer y escribir)
#include <sstream> // procesar strings
#include <algorithm>//buscar y eliminar elementos de vectores

// Definición de la clase Usuario
class Usuario {
public:
    std::string nombre;
    std::vector<std::string> librosPrestados; //lista de titulos prestados

    Usuario() {}
    Usuario(const std::string& n) : nombre(n) {}//nombre del usuario 

    void mostrarDetalles() const {
        std::cout << "Nombre: " << nombre << "\n"
                  << "Libros prestados: " << librosPrestados.size() << std::endl;
        
        if (!librosPrestados.empty()) { // el empty verifica si está vacio o no
            std::cout << "Títulos prestados:\n";
            for (const auto& titulo : librosPrestados) {
                std::cout << "  - " << titulo << "\n";
            }
        }
    }
//Añadir libro a la lista del usuario
    void prestarLibro(const std::string& titulo) {
        librosPrestados.push_back(titulo);
    }
//elimina el libro de la lista del usuario
    bool devolverLibro(const std::string& titulo) {
        auto it = std::find(librosPrestados.begin(), librosPrestados.end(), titulo);
        if (it != librosPrestados.end()) {
            librosPrestados.erase(it);//cuando lo encuentra lo elimina
            return true;
        }
        return false;
    }
};

// Clase libro
class Libro {
public:
    std::string titulo;
    std::string autor;
    int anioPublicacion;
    bool estaDisponible;//true=disponible, false = prestado
    std::string usuarioPrestamo; // Nombre del usuario que tiene el libro

    Libro() : anioPublicacion(0), estaDisponible(true), usuarioPrestamo("") {}

    void mostrarDetallesCompletos() const {
        std::cout << "Título: " << titulo << "\n"
                  << "Autor: " << autor << "\n"
                  << "Año de publicación: " << anioPublicacion << "\n"
                  << "Estado: " << (estaDisponible ? "Disponible" : "Prestado");
        
        if (!estaDisponible && !usuarioPrestamo.empty()) {//muestra quien de los usuarios lo prestó
            std::cout << " (Usuario: " << usuarioPrestamo << ")";
        }
        std::cout << std::endl;
    }
};

// Clase Biblioeta 
class Biblioteca {
private:
    std::vector<Libro> coleccion;
    std::vector<Usuario> usuarios;
    std::string archivoLibros;//archivo donde se guardan los libros
    std::string archivoUsuarios;//archivo donde se guardan los usuarios

    // limpiar el buffer 
    void limpiarBuffer() {
        std::cin.clear();
        std::string trash;
        std::getline(std::cin, trash);
    }

public:
    Biblioteca(const std::string& archLibros = "libros.txt", 
               const std::string& archUsuarios = "usuarios.txt") 
        : archivoLibros(archLibros), archivoUsuarios(archUsuarios) {
        cargarDatos();
    }

    ~Biblioteca() { //guarda datos automaticamente al salir
        guardarDatos();
    }

    // Gestionar libros - prevenir duplicados
    bool agregarLibro(const Libro& nuevoLibro) {
        // Verificar duplicados
        for (const auto& libro : coleccion) {
            if (libro.titulo == nuevoLibro.titulo) {
                std::cout << "Error: Ya existe un libro con ese título.\n";
                return false;//no agrega el libro
            }
        }
        
        coleccion.push_back(nuevoLibro); //aquí si agrega el libro
        std::cout << "Libro añadido al inventario.\n";
        return true;
    }

    void mostrarInventario() const {
        if (coleccion.empty()) {
            std::cout << "Inventario vacío.\n";
            return;
        }
        std::cout << "\n INVENTARIO \n";
        //muestra cada libro con el numero en el que han sido registrados
        for (size_t i = 0; i < coleccion.size(); ++i) {
            std::cout << "#" << (i + 1) << "\n";
            coleccion[i].mostrarDetallesCompletos();
            std::cout << "---\n";
        }
    }

    Libro* buscarLibro(const std::string& tituloBuscado) {
        for (auto& libro : coleccion) {
            if (libro.titulo == tituloBuscado) {
                return &libro;
            }
        }
        return nullptr;
    }

    // Gestion de usuarios
    bool registrarUsuario(const Usuario& nuevoUsuario) {
        // Verificar que no exista el nombre
        for (const auto& usuario : usuarios) {
            if (usuario.nombre == nuevoUsuario.nombre) {
                std::cout << "Error: Ya existe un usuario con ese nombre.\n";
                return false;
            }
        }
        
        usuarios.push_back(nuevoUsuario);//push back para añadir elemento al vector
        std::cout << "Usuario registrado exitosamente.\n";
        return true;
    }

    void mostrarUsuarios() const {
        if (usuarios.empty()) {
            std::cout << "No hay usuarios registrados.\n";
            return;
        }
        
        std::cout << "\n USUARIOS REGISTRADOS\n";
        for (size_t i = 0; i < usuarios.size(); ++i) {
            std::cout << "#" << (i + 1) << "\n";
            usuarios[i].mostrarDetalles();
            std::cout << "---\n";
        }
    }

    Usuario* buscarUsuario(const std::string& nombre) {//busca el usuario por el nombre
        for (auto& usuario : usuarios) {
            if (usuario.nombre == nombre) {
                return &usuario;
            }
        }
        return nullptr;
    }

    // Prestar libro
    void prestarLibro(const std::string& tituloPrestamo, const std::string& nombreUsuario) {
        Libro* libro = buscarLibro(tituloPrestamo);
        if (!libro) {
            std::cout << "No se encontró el libro.\n";
            return;
        }
        
        if (!libro->estaDisponible) {//verificar disponibilidad del libro
            std::cout << "El libro ya está prestado.\n";
            return;
        }

        Usuario* usuario = buscarUsuario(nombreUsuario);
        if (!usuario) {
            std::cout << "Usuario no encontrado.\n";
            return;
        }
        //realiza el prestamo del libro

        libro->estaDisponible = false;//marcar como prestado
        libro->usuarioPrestamo = nombreUsuario;//Asignar a un usuario
        usuario->prestarLibro(tituloPrestamo);//añadir a la lista del usuario
        std::cout << "Libro prestado a " << usuario->nombre << ".\n";
    }

    void devolverLibro(const std::string& tituloDevolucion, const std::string& nombreUsuario) {
        Libro* libro = buscarLibro(tituloDevolucion);
        if (!libro) {
            std::cout << "No se encontró el libro.\n";
            return;
        }
        
        if (libro->estaDisponible) {//verifica si está prestado
            std::cout << "Ese libro no estaba prestado.\n";
            return;
        }

        if (libro->usuarioPrestamo != nombreUsuario) {//verifica que lo tenga el usuario correcto
            std::cout << "Este libro no fue prestado a este usuario.\n";
            return;
        }

        Usuario* usuario = buscarUsuario(nombreUsuario);
        if (usuario && usuario->devolverLibro(tituloDevolucion)) {
            libro->estaDisponible = true;
            libro->usuarioPrestamo = "";
            std::cout << "Libro devuelto exitosamente.\n";
        } else {
            std::cout << "Error en la devolución.\n";
        }
    }

    // Guardar datos
    void guardarDatos() const {
        // Guardar libros en: titulo|autor|año|disponible|usuario
        std::ofstream archivoL(archivoLibros); //ofstream es para escribir los datos en los archivos txt
        if (archivoL.is_open()) {
            for (const auto& libro : coleccion) {
                archivoL << libro.titulo << " |"
                        << libro.autor << " |"
                        << libro.anioPublicacion << " |"
                        << (libro.estaDisponible ? 1 : 0) << " |"
                        << libro.usuarioPrestamo << "\n";
            }
            archivoL.close();
        }

        // Guardar usuarios
        std::ofstream archivoU(archivoUsuarios);//escribir datos en usuarios
        if (archivoU.is_open()) {
            for (const auto& usuario : usuarios) {
                archivoU << usuario.nombre << "|";
                for (size_t i = 0; i < usuario.librosPrestados.size(); ++i) {
                    if (i > 0) archivoU << ",";
                    archivoU << usuario.librosPrestados[i];
                }
                archivoU << "\n";
            }
            archivoU.close();
        }
    }

    void cargarDatos() {
        // Cargar libros
        std::ifstream archivoL(archivoLibros);
        if (archivoL.is_open()) {
            std::string linea;
            while (std::getline(archivoL, linea)) {
                std::stringstream ss(linea);
                std::string item;
                Libro libro;
                
                if (std::getline(ss, item, '|')) libro.titulo = item; //getline para leer una linea y almacenarla en string
                if (std::getline(ss, item, '|')) libro.autor = item;
                if (std::getline(ss, item, '|')) libro.anioPublicacion = std::stoi(item);
                if (std::getline(ss, item, '|')) libro.estaDisponible = (item == "1");
                if (std::getline(ss, item, '|')) libro.usuarioPrestamo = item;
                
                coleccion.push_back(libro);
            }
            archivoL.close();
        }

        // Cargar usuarios
        std::ifstream archivoU(archivoUsuarios);
        if (archivoU.is_open()) {
            std::string linea;
            while (std::getline(archivoU, linea)) {
                std::stringstream ss(linea);
                std::string item;
                Usuario usuario;
                
                if (std::getline(ss, item, '|')) usuario.nombre = item;
                if (std::getline(ss, item, '|')) {
                    if (!item.empty()) {
                        std::stringstream libros(item);
                        std::string libro;
                        while (std::getline(libros, libro, ',')) {
                            usuario.librosPrestados.push_back(libro);
                        }
                    }
                }
                
                usuarios.push_back(usuario);
            }
            archivoU.close();
        }
    }
};

// --- Función Principal con el Menú ---
int main() {
    Biblioteca miBiblioteca;
    int opcion = 0;

    // Agregar libro inicial si la colección está vacía
    Libro libroInicial;
    libroInicial.titulo = "El Hobbit";
    libroInicial.autor = "J.R.R. Tolkien";
    libroInicial.anioPublicacion = 1937;
    miBiblioteca.agregarLibro(libroInicial);//agregaLibro previene duplicados

    while (opcion != 7) { //continua hasta que el usuario decida salir
        std::cout << "\n BIBLIOTECA DIGITAL " << std::endl;
        std::cout << "1. Añadir libro" << std::endl;
        std::cout << "2. Mostrar inventario" << std::endl;
        std::cout << "3. Registrar usuario" << std::endl;
        std::cout << "4. Mostrar usuarios" << std::endl;
        std::cout << "5. Prestar libro" << std::endl;
        std::cout << "6. Devolver libro" << std::endl;
        std::cout << "7. Salir" << std::endl;
        std::cout << "Seleccione una opción: ";
        
        std::cin >> opcion;
        std::cin.ignore(); // Limpiar buffer (el \n que queda despues del numero)

        switch(opcion) {
            case 1: {
                Libro nuevo;//crea libro temporal
                std::cout << "Título: ";
                std::getline(std::cin, nuevo.titulo);

                std::cout << "Autor: ";
                std::getline(std::cin, nuevo.autor);

                std::cout << "Año de publicación: ";
                while (!(std::cin >> nuevo.anioPublicacion)) {//si el usuario no ingresa numero lo tiene que repetir
                    std::cout << "Entrada inválida. Ingrese un número: ";
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');//descarta entrada invalida
                }
                std::cin.ignore();

                nuevo.estaDisponible = true;//por defecto el nuevo libro está disponible
                miBiblioteca.agregarLibro(nuevo);//agrega libro, si ya está falla
                break;
            }
            case 2:
                miBiblioteca.mostrarInventario();
                break;
            
            case 3: {
                Usuario nuevoUsuario;//crea usuario temporal
                std::cout << "Nombre del usuario: ";
                std::getline(std::cin, nuevoUsuario.nombre);
                
                miBiblioteca.registrarUsuario(nuevoUsuario);//verifica duplicados
                break;
            }
            case 4:
                miBiblioteca.mostrarUsuarios();//muestra los usuarios y cuales han prestado
                break;
                
            case 5: {
                std::string titulo, nombreUsuario;
                std::cout << "Título a prestar: ";
                std::getline(std::cin, titulo);
                
                std::cout << "Nombre del usuario: ";
                std::getline(std::cin, nombreUsuario);
                
                miBiblioteca.prestarLibro(titulo, nombreUsuario);
                break;
            }
            case 6: {
                std::string titulo, nombreUsuario;
                std::cout << "Título a devolver: ";
                std::getline(std::cin, titulo);
                
                std::cout << "Nombre del usuario: ";
                std::getline(std::cin, nombreUsuario);
                
                miBiblioteca.devolverLibro(titulo, nombreUsuario);//verifica que el libro esté prestado al usuario
                break;
            }
           
                
            case 7:
                std::cout << "Guardando datos y saliendo...\n";
                break;
                
            default:
                std::cout << "Opción inválida.\n";
                break;
        }
    }

    return 0;//al terminar main se guardan los datos en los archivos txt
}
