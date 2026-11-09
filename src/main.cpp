#include <iostream>
#include "../include/test.hpp"
#include "../include/catopt.hpp"

int main(){

    std::cout << "Projet CatOpt :\n\n";
    std::cout << "1. Un morphisme scalaire\n";

    const catopt::Morphism f({catopt::make_add(1)});
    const catopt::Morphism g({catopt::make_square()});
    
    std::cout << "  f = Add(1)\n";
    std::cout << "  g = Square\n";
    std::cout << "  g ○ f = " << catopt::to_string(catopt::compose(g, f)) << std::endl;
    std::cout << "  (g ○ f)(3) = " << catopt::evaluate(catopt::compose(g, f), 3) << "\n\n";

    std::cout << "2. Une reecriture arithmetique\n";

    const catopt::Morphism av({catopt::make_add(1), catopt::make_add(2), catopt::make_square()});
    
    std::cout << "  Avant : " << catopt::to_string(av) << std::endl;
    std::cout << "  Apres : " << catopt::to_string(catopt::simplify(av)) << std::endl;
    std::cout << "  Loi   : A_b ○ A_a = A_(a+b)\n";
    std::cout << "  Entree 3 : " << catopt::evaluate(av, 3) << " dans les deux cas\n\n";

    std::cout << "3. Une reecriture par fonctorialite\n";

    const catopt::MapPipeline h{{f, g}};
    catopt::Values input = {0, 1, 2, 3};
    catopt::Values output = catopt::evaluate(h, input);

    std::cout << "  Avant : " << catopt::to_string(h) << std::endl;
    std::cout << "  Apres : " << catopt::to_string(catopt::fuse_maps(h)) << std::endl;
    std::cout << "  Loi   : F(g) ○ F(f) = F(g ○ f)\n";
    std::cout << "  Entree : [" << input[0] << ", " << input[1] << ", " << input[2] << ", " << input[3] << "]\n";
    std::cout << "  Resultat : [" << output[0] << ", " << output[1] << ", " << output[2] << ", " << output[3] << "]\n";
    std::cout << "  Parcours map : " << h.stages.size() << " -> " << catopt::fuse_maps(h).stages.size() << "\n\n";


    std::cout << "4. Vérification\n";

    run_tests_categorie();
    run_tests_optimiseur();

    std::cout << "Tout les test sont passé avec succes !\n";

    return 0;
}