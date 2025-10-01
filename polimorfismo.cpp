#include <iostream>

template <typename T> // typename es algo genérico 
T suma(T a, T b){
    return a +b;
}

int main(){
    std::cout <<"Suma de enteros: "<< suma(5,7)<<std::endl;
    std::cout <<"Suma de enteros: "<< suma(5,7)<<std::endl;
    std::cout <<"Suma de strings: "<< suma(std::string("hola"),std::string("mundo"))<<std::endl;//da error sin el std 

    return 0;

}