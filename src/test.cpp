#include "../include/test.hpp"
#include "../include/categorie.hpp"

#include <cassert>
#include <iostream>
#include <limits>

void run_tests(){
    
    assert(square_value(add_value(3, 1)) == 16);
    assert(add_value(square_value(3), 1) == 10);
    assert(add_value(std::numeric_limits<Scalar>::max(), 1) == 0);

    const auto f = make_add(1);
    const auto g = make_square();
    assert(evaluate(f, 3) == 4);
    assert(evaluate(g, 4) == 16);
    assert(to_string(f) == "Add(1)");

    const auto m = identity();
    Morphism m2;
    m2.steps.push_back(make_add(1));
    m2.steps.push_back(make_square());
    assert(evaluate(m, 1) == 1);
    assert(evaluate(m2, 3) == 16);
    assert(to_string(m) == "Id");
    assert(to_string(m2) == "Add(1) >> Square");

    Morphism m3{{make_square(), make_add(4), make_square()}};
    assert(evaluate(m3, 3) == 169);

    Morphism m4 = compose(m2, m3);
    assert(evaluate(m4, 3) == 28900);

    Morphism a{{make_square(), make_add(4), make_square()}};
    Morphism a2{{make_add(4), make_square()}};
    Morphism a3{{make_square(), make_add(3), make_square()}};
    Morphism a4{{make_square(), make_add(4), make_square()}};
    Morphism a5{{make_square(), make_add(4), make_square(), make_add(3)}};
    assert(!same_syntax(a, a2));
    assert(!same_syntax(a, a3));
    assert(same_syntax(a, a4));
    assert(!same_syntax(a, a5));

    const auto f2 = lift(make_add(1));
    const auto g2 = lift(make_square());
    const Scalar x{3};
    assert(evaluate(compose(g2, f2), x) == evaluate(g2, evaluate(f2, x)));

    assert(same_syntax(compose(identity(), a), a));
    assert(same_syntax(compose(a, identity()), a));
    assert(same_syntax(compose(compose(a, a2), a3), compose(a, compose(a2, a3))));

    std::cout << "Tout les test sont passé avec succes !\n";
}