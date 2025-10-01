#include <iostream>
#include <string>

//Clase base 
class Animal{
public:
        virtual void hacerSonido() const{//virtual es una palabra clave que permite que la función sea sobreescrita, se puede agarrar y cambiar después
                std::cout << "El animal hace un sonido generico" << std::endl; //animal -> objetos de hace clase animal

        }
        //Destructor 
        virtual ~Animal() {}//cuando no se use se destruye para liberar memoria 
}

//Clase derivada 1
class Perro : public Animal{//Animal tiene que ser exactamente igual 
public: 
        void hacerSonido() const override{//override para hacerlo legible, importar la función de arriba y la vamos a cmabiar 
            std:: cout <<"El perro ladra: Guau" << std::endl;


        }
}

//Clase derivada 2
class Gato : public Animal{
public: 
        void hacerSonido() const override{
            std:: cout <<"El gato maulla: Miau" << std::endl;
            

        }
}

void sonido(const Animal& animal){
    animal.hacerSonido();
}


int main(){

    Animal miAnimal;
    Perro miPerro;
    Gato miGato;

    miAnimal.hacerSonido();

    sonido(miAnimal);
    sonido(miPerro);
    sonido(miGato);

    return 0;

}



