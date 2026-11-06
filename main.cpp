#include <iostream>
#include <cassert>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using Scalar = std::uint64_t;


Scalar identity_value(Scalar x){
    return x;
}

Scalar add_value(Scalar x, Scalar constant){
    return x + constant;
}

Scalar square_value(Scalar x){
    return x * x;
}

enum class PrimitiveKind{
    AddConstant,
    Square
};

struct Primitive{
    PrimitiveKind kind;
    Scalar constant;
};

Primitive make_add(Scalar constant){
    Primitive p;
    p.constant = constant;
    p.kind = PrimitiveKind::AddConstant;

    return p;
}

Primitive make_square(){
    Primitive p;
    p.constant = 0;
    p.kind = PrimitiveKind::Square;

    return p;
}

Scalar evaluate(const Primitive& primitive, Scalar x){
    switch (primitive.kind)
    {
    case PrimitiveKind::AddConstant :
        x = add_value(x, primitive.constant);
        break;

    case PrimitiveKind::Square :
        x = square_value(x);
        break;

    default:
        throw std::logic_error("primitive n'a pas de PrimitiveKind.");
    }
    return x;
}

std::string to_string(const Primitive& primitive){
    std::string s;

    switch (primitive.kind)
    {
    case PrimitiveKind::AddConstant :
        s = "Add(" + std::to_string(primitive.constant) + ")";
        break;

    case PrimitiveKind::Square :
        s = "Square";
        break;
    
    default :
        throw std::logic_error("primitive n'a pas de PrimitiveKind.");
    }

    return s;
}

struct Morphism{
    std::vector<Primitive> steps;
};

Morphism identity(){
    Morphism m;

    return m;
}

Morphism lift(Primitive primitive){
    Morphism m;
    m.steps.push_back(primitive);

    return m;
}

Scalar evaluate(const Morphism& morphism, Scalar x){
    for(const Primitive& f : morphism.steps){
        x = evaluate(f, x);
    }

    return x;
}

std::string to_string(const Morphism& morphism){
    if(morphism.steps.empty()) return "Id";
    
    std::string s;
    for(size_t i = 0; i < morphism.steps.size() - 1; i++){
        s += to_string(morphism.steps[i]) + " >> ";
    }
    s += to_string(morphism.steps[morphism.steps.size() - 1]);
    
    return s;
}

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

    std::cout << "Tout les test sont passé avec succes !\n";
}

int main(){

    std::cout << "CatOpt - premier morphisme\n";
    run_tests();

    return 0;
}