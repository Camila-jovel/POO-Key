#include <iostream>
#include <string>

class Instrumentos {
protected:
    std::string tipo;
    std::string nombre;
    std::string material;

public:
    Instrumentos(const std::string& nombre, const std::string& tipo, const std::string& material) {
        this->nombre = nombre;
        this->tipo = tipo;
        this->material = material;
        std::cout << "Se ha creado un instrumento" << std::endl;
    }

    void afinar() {
        std::cout << "El instrumento " << nombre 
                  << " de tipo " << tipo
                  << " de material " << material
                  << " está siendo afinado" << std::endl;
    }

    void tocar() {
        std::cout << "El instrumento " << nombre 
                  << " toca música" << std::endl;
    }
};

// Clase hija: Violin
class Violin : public Instrumentos {
private:
    int numeroCuerdas;
    std::string tamanio;

public:
    Violin(const std::string& nombre, const std::string& tipo, const std::string& material,
           const std::string& tamanioViolin, int numeroCuerdas)
        : Instrumentos(nombre, tipo, material) {  // Llamada correcta al constructor padre
        this->tamanio = tamanioViolin;
        this->numeroCuerdas = numeroCuerdas;
        std::cout << "Se ha especificado que es un instrumento: Violín." << std::endl;
    }

    void mostrarInfo() {
        std::cout << "Violín de tamaño " << tamanio
                  << " con " << numeroCuerdas << " cuerdas." << std::endl;
    }
};

int main() {
    Violin miViolin("Stradivarius", "Cuerda frotada", "Madera", "Grande", 4);
    miViolin.afinar();
    miViolin.tocar();
    miViolin.mostrarInfo();

    std::cout << "-----------------------------" << std::endl;

    return 0;
}