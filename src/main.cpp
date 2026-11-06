#include <iostream>
#include "../include/test.hpp"



int main(){

    std::cout << "Projet CatOpt\n";
    run_tests_categorie();
    run_tests_optimiseur();

    std::cout << "Tout les test sont passé avec succes !\n";

    return 0;
}