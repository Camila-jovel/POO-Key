#include <iostream>
#include <string>
using namespace std;
#include <vector>

class Vehiculo{
protected:
    string marca;
    string modelo;
    int año;
    double precioBase;

public:
    Vehiculo(string _marca, string _modelo, int _año, double _precioBase){
        marca = _marca;
        modelo = _modelo;
        año =_año;
        precioBase=_precioBase;


    }
    virtual double precioFinal(){
        return precioBase;
    }
    //Método virtual
     virtual void show_type(){ 
        cout << "Generic vehiculo resource." << endl;
    }
    
    string getMarca() const { return marca; }
    string getModelo() const { return modelo; }
    virtual ~Vehiculo() {}
};

class Auto: public Vehiculo{
private:
    int numeroPuertas;
public:
    Auto(string _marca, string _modelo, int _año, double _precioBase, int _numeroPuertas)
        :Vehiculo(_marca, _modelo,_año,_precioBase)

    {
        numeroPuertas=_numeroPuertas;
    }
    void show_type() override {
        cout << "Es un auto" << endl;
    }
    double precioFinal()  override{ 
        return precioBase + (precioBase*0.15);
    }

};

class Moto: public Vehiculo{
private:
    int cilindrada;
public:
    Moto(string _marca, string _modelo, int _año, double _precioBase, int _cilindrada)
        :Vehiculo(_marca, _modelo,_año,_precioBase)
    {
        cilindrada= _cilindrada;
    }
     void show_type() override {
        cout << "Es un moto" << endl;
    }
    double precioFinal()  override{ 
        return precioBase + (precioBase*0.10);
    }

};
int main() {
    vector<Vehiculo*> vehiculos;

    
    vehiculos.push_back(new Auto("Toyota", "Corolla", 2022, 20000, 4));
    vehiculos.push_back(new Moto("Yamaha", "R3", 2021, 5000, 321));

  
    for (Vehiculo* v : vehiculos) {
        cout << "Marca: " << v->getMarca()
             << " Modelo: " << v->getModelo()
             << " Precio Final: $" << v->precioFinal()
             << endl;
    }

    
    for (Vehiculo* v : vehiculos) {
        delete v;
    }

    return 0;
}