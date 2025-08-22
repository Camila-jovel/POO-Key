#include <iostream>
#include <string>

class Libro{
public:
    std::string titulo;
    std::string autor;
    int anioPublicacion;

    void mostrarInformacion() {
        std::cout << "---------" << std::endl;
        std::cout << "Titulo: " << titulo << std::endl;
        std::cout << "Autor : " << autor << std::endl;
        std::cout << "Año de Publicación: " << anioPublicacion << std::endl;
    }
};



int main() {
    Libro libro1;
    Libro libro2;
    Libro libro3;
    Libro libro4;
    Libro libro5;
    Libro libro6;

    libro1.titulo = "Cien años de soledad";
    libro1.autor = "Gabriel García Márquez";
    libro1.anioPublicacion = 1967;

    libro2.titulo = "El principito ";
    libro2.autor = "Saint-Exupéry";
    libro2.anioPublicacion = 1950;

    
    libro3.titulo = "El señor de los anillos";
    libro3.autor = "JRR Tolkien";
    libro3.anioPublicacion = 2005;

    
    libro4.titulo = "Las aventuras de Sherlock Holmes";
    libro4.autor = "Arthur Conan Doyle";
    libro4.anioPublicacion = 1970;

    libro5.titulo = "El temor de un hombre sabio";
    libro5.autor = "Patrick Rothfuss";
    libro5.anioPublicacion = 2006;

    libro6.titulo = "El bosón de Higgs no te hará la cama";
    libro6.autor = "Javier Santaolalla";
    libro6.anioPublicacion = 2010;


    
    std::cout << "Información de la biblioteca:" << std::endl;
    libro1.mostrarInformacion();
    libro2.mostrarInformacion();
    libro3.mostrarInformacion();
    libro4.mostrarInformacion();
    libro5.mostrarInformacion();
    libro6.mostrarInformacion();

    return 0;
}