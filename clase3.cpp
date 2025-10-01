#include <iostream>

void imprimir(int numero){
    std::cout << "Número entero: " << numero << std:: endl;
}

void imprimir(double numero){
    std::cout << "Número decimal: " << numero << std::endl;

}

void imprimir(const std:: string& texto){
    std::cout << "texto: "<< texto << std::endl;
 }
int main(){
    imprimir(19);
    imprimir(19.5);
    imprimir("Hola Mundo");
    double d = 2.9;

    int i = static_cast<int>(d);

    imprimir(d);
    imprimir(i);
    return 0;
}