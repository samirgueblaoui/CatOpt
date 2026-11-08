#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace catopt{

using Scalar = std::uint64_t;
using Values = std::vector<Scalar>;


Scalar identity_value(Scalar x);

Scalar add_value(Scalar x, Scalar constant);

Scalar square_value(Scalar x);

enum class PrimitiveKind{
    AddConstant,
    Square
};

class Primitive {
public:
    static Primitive add(Scalar constant);
    static Primitive square();

    PrimitiveKind kind() const;
    Scalar constant() const;

private:
    Primitive(PrimitiveKind kind, Scalar constant);

    PrimitiveKind kind_;
    Scalar constant_;
};

Primitive make_add(Scalar constant);

Primitive make_square();

Scalar evaluate(const Primitive& primitive, Scalar x);

std::string to_string(const Primitive& primitive);

class Morphism{
public:
    Morphism();
    Morphism(const std::vector<Primitive>& steps);

    const std::vector<Primitive>& steps() const;

private:
    std::vector<Primitive> steps_;
};

Morphism identity();

Morphism lift(Primitive primitive);

Scalar evaluate(const Morphism& morphism, Scalar x);

std::string to_string(const Morphism& morphism);

Morphism compose(const Morphism& g, const Morphism& f);

bool same_syntax(const Morphism& a, const Morphism& b);

Morphism simplify(const Morphism& input);

Values map_values(const Morphism& f, const Values& input);

struct MapPipeline{
    std::vector<Morphism> stages;
};

Values evaluate(const MapPipeline& pipeline, const Values& input);

std::string to_string(const MapPipeline& pipeline);

MapPipeline fuse_maps(const MapPipeline& input);

}
