#include <iostream>
#include <string>

class Vehiculo {
protected:
    std::string marca;
    std::string modelo;
    int año;
    double precioBase;

public:
    Vehiculo(const std::string& marca, const std::string& modelo, const int& año,const double&precioBase){ 
       
    }

    void calcularPrecioFinal(){ 
      
    }
    void mostrar();
    

   
};

// Clase hija: Auto
class Auto : public Vehiculo {
private:
    int numeroPuertas;

public:
    Auto(const string& marca, const string& modelo, int año, double precioBase, int numeroPuertas)
        : Vehiculo(marca, modelo, año, precioBase), numeroPuertas(numeroPuertas) {}

    double calcularPrecioFinal() const override {
        return precioBase + (precioBase * 0.15); // Impuesto 15%
    }
};
//Clase hija: moto 
class moto:public Vehiculo{ 
private:
    int cilindrada;
public:
  moto(const std::string& marca, const std::string& modelo, const int& año,const double&precioBase)
        
        : Vehiculo(marca,modelo,año,precioBase){//mada correcta al constructor padre
        this->cilindrada=cilindrada;
        std::cout << "Se ha especificado que es un moto." << std::endl;
    }

};





int main() {
   
    const double TASA_IMPUESTO = 0.15;

   
    double precioBase;

    std::cout << "Ingrese el precio base del vehículo: ";
    std::cin >> precioBase;

    double montoImpuesto = precioBase * TASA_IMPUESTO;

  
    std::cout << "El monto del impuesto es: " << montoImpuesto << std::endl;

    return 0;
}

