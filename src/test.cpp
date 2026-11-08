#include "../include/test.hpp"
#include "../include/catopt.hpp"

#include <cassert>
#include <iostream>
#include <limits>

void run_tests_categorie(){
    
    assert(catopt::square_value(catopt::add_value(3, 1)) == 16);
    assert(catopt::add_value(catopt::square_value(3), 1) == 10);
    assert(catopt::add_value(std::numeric_limits<catopt::Scalar>::max(), 1) == 0);

    const auto f = catopt::make_add(1);
    const auto g = catopt::make_square();
    assert(evaluate(f, 3) == 4);
    assert(evaluate(g, 4) == 16);
    assert(to_string(f) == "Add(1)");

    const auto m = catopt::identity();
    catopt::Morphism m2({catopt::make_add(1), catopt::make_square()});
    assert(evaluate(m, 1) == 1);
    assert(evaluate(m2, 3) == 16);
    assert(to_string(m) == "Id");
    assert(to_string(m2) == "Add(1) >> Square");

    catopt::Morphism m3{{catopt::make_square(), catopt::make_add(4), catopt::make_square()}};
    assert(evaluate(m3, 3) == 169);

    catopt::Morphism m4 = compose(m2, m3);
    assert(evaluate(m4, 3) == 28900);

    catopt::Morphism a{{catopt::make_square(), catopt::make_add(4), catopt::make_square()}};
    catopt::Morphism a2{{catopt::make_add(4), catopt::make_square()}};
    catopt::Morphism a3{{catopt::make_square(), catopt::make_add(3), catopt::make_square()}};
    catopt::Morphism a4{{catopt::make_square(), catopt::make_add(4), catopt::make_square()}};
    catopt::Morphism a5{{catopt::make_square(), catopt::make_add(4), catopt::make_square(), catopt::make_add(3)}};
    assert(!same_syntax(a, a2));
    assert(!same_syntax(a, a3));
    assert(same_syntax(a, a4));
    assert(!same_syntax(a, a5));

    const auto f2 = catopt::lift(catopt::make_add(1));
    const auto g2 = catopt::lift(catopt::make_square());
    const catopt::Scalar x{3};
    assert(evaluate(compose(g2, f2), x) == evaluate(g2, evaluate(f2, x)));

    assert(same_syntax(compose(catopt::identity(), a), a));
    assert(same_syntax(compose(a, catopt::identity()), a));
    assert(same_syntax(compose(compose(a, a2), a3), compose(a, compose(a2, a3))));
}

void run_tests_optimiseur(){
    catopt::Morphism m3{{catopt::make_add(0)}};
    assert(same_syntax(simplify(m3), catopt::identity()));
    assert(same_syntax(simplify(catopt::identity()), catopt::identity()));

    catopt::Morphism m4{{catopt::make_add(1), catopt::make_add(2)}};
    catopt::Morphism m4s{{catopt::make_add(3)}};
    assert(same_syntax(simplify(m4), m4s));

    catopt::Morphism m5{{catopt::make_add(1), catopt::make_add(2), catopt::make_add(3)}};
    catopt::Morphism m5s{{catopt::make_add(6)}};
    assert(same_syntax(simplify(m5), m5s));

    catopt::Morphism m6{{catopt::make_add(1), catopt::make_square(), catopt::make_add(2)}};
    assert(same_syntax(simplify(m6), m6));

    catopt::Morphism m7{{catopt::make_add(std::numeric_limits<catopt::Scalar>::max()), catopt::make_add(1)}};
    assert(same_syntax(simplify(m7), catopt::identity()));

    catopt::Morphism m8{{catopt::make_add(0), catopt::make_square(), catopt::make_add(2), catopt::make_add(3)}};
    catopt::Morphism m8s{{catopt::make_square(), catopt::make_add(5)}};
    assert(same_syntax(simplify(m8), m8s));
    assert(same_syntax(simplify(simplify(m8)), simplify(m8)));


    catopt::Morphism f{{catopt::make_add(0), catopt::make_square(), catopt::make_add(2), catopt::make_add(3)}};
    catopt::Morphism g{{catopt::make_square(), catopt::make_add(4), catopt::make_square()}};
    catopt::Values vgf{841, 1600, 7225, 40000};
    catopt::Values empty;
    catopt::Values input{0, 1, 2, 3};
    catopt::Values v;
    assert(map_values(f, empty) == v);
    assert(input.size() == map_values(f, input).size());


    assert(map_values(catopt::identity(), input) == input);
    assert(map_values(compose(g, f), input) == vgf);
    assert(map_values(compose(g, f), input) == map_values(g, map_values(f, input)));


    catopt::MapPipeline mp{{f, g}};
    catopt::MapPipeline mp2{{}};
    catopt::MapPipeline mp3{{f, g, catopt::identity()}};
    catopt::MapPipeline mp4{{catopt::identity()}};
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