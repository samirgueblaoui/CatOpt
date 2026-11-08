#include "../include/test.hpp"
#include "../include/categorie.hpp"
#include "../include/optimiseur.hpp"

#include <cassert>
#include <iostream>
#include <limits>

void run_tests_categorie(){
    
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
}

void run_tests_optimiseur(){
    Morphism m3{{make_add(0)}};
    assert(same_syntax(simplify(m3), identity()));
    assert(same_syntax(simplify(identity()), identity()));

    Morphism m4{{make_add(1), make_add(2)}};
    Morphism m4s{{make_add(3)}};
    assert(same_syntax(simplify(m4), m4s));

    Morphism m5{{make_add(1), make_add(2), make_add(3)}};
    Morphism m5s{{make_add(6)}};
    assert(same_syntax(simplify(m5), m5s));

    Morphism m6{{make_add(1), make_square(), make_add(2)}};
    assert(same_syntax(simplify(m6), m6));

    Morphism m7{{make_add(std::numeric_limits<Scalar>::max()), make_add(1)}};
    assert(same_syntax(simplify(m7), identity()));

    Morphism m8{{make_add(0), make_square(), make_add(2), make_add(3)}};
    Morphism m8s{{make_square(), make_add(5)}};
    assert(same_syntax(simplify(m8), m8s));
    assert(same_syntax(simplify(simplify(m8)), simplify(m8)));


    Morphism f{{make_add(0), make_square(), make_add(2), make_add(3)}};
    Morphism g{{make_square(), make_add(4), make_square()}};
    Values vgf{841, 1600, 7225, 40000};
    Values empty;
    Values input{0, 1, 2, 3};
    Values v;
    assert(map_values(f, empty) == v);
    assert(input.size() == map_values(f, input).size());


    assert(map_values(identity(), input) == input);
    assert(map_values(compose(g, f), input) == vgf);
    assert(map_values(compose(g, f), input) == map_values(g, map_values(f, input)));


    MapPipeline mp{{f, g}};
    MapPipeline mp2{{}};
    MapPipeline mp3{{f, g, identity()}};
    MapPipeline mp4{{identity()}};
    assert(evaluate(mp, input) == vgf);
    assert(evaluate(mp2, input) == input);
    assert(evaluate(mp4, input) == input);
    assert(evaluate(mp, empty).empty());
    assert(to_string(mp) == "Map(Add(0) >> Square >> Add(2) >> Add(3)) >> Map(Square >> Add(4) >> Square)");
    assert(to_string(mp2) == "IdList");
    assert(to_string(mp3) == "Map(Add(0) >> Square >> Add(2) >> Add(3)) >> Map(Square >> Add(4) >> Square) >> Map(Id)");
    assert(to_string(mp4) == "Map(Id)");

    assert(evaluate(fuse_maps(mp3), input) == evaluate(mp3, input));
    assert(evaluate(fuse_maps(mp2), input) == evaluate(mp2, input));
    assert(to_string(fuse_maps(mp3)) == "Map(Add(0) >> Square >> Add(2) >> Add(3) >> Square >> Add(4) >> Square)");

}