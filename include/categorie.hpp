#pragma once

#include <cstdint>
#include <string>
#include <vector>

using Scalar = std::uint64_t;


Scalar identity_value(Scalar x);

Scalar add_value(Scalar x, Scalar constant);

Scalar square_value(Scalar x);

enum class PrimitiveKind{
    AddConstant,
    Square
};

struct Primitive{
    PrimitiveKind kind;
    Scalar constant;
};

Primitive make_add(Scalar constant);

Primitive make_square();

Scalar evaluate(const Primitive& primitive, Scalar x);

std::string to_string(const Primitive& primitive);

struct Morphism{
    std::vector<Primitive> steps;
};

Morphism identity();

Morphism lift(Primitive primitive);

Scalar evaluate(const Morphism& morphism, Scalar x);

std::string to_string(const Morphism& morphism);

Morphism compose(const Morphism& g, const Morphism& f);

bool same_syntax(const Morphism& a, const Morphism& b);