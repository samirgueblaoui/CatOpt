#include "../include/catopt.hpp"

#include <stdexcept>

namespace catopt{

Scalar identity_value(Scalar x){
    return x;
}

Scalar add_value(Scalar x, Scalar constant){
    return x + constant;
}

Scalar square_value(Scalar x){
    return x * x;
}

Primitive::Primitive(PrimitiveKind kind, Scalar constant)
    : kind_(kind), constant_(constant) {
}

Primitive Primitive::add(Scalar constant) {
    return Primitive(PrimitiveKind::AddConstant, constant);
}

Primitive Primitive::square() {
    return Primitive(PrimitiveKind::Square, 0);
}

PrimitiveKind Primitive::kind() const {
    return kind_;
}

Scalar Primitive::constant() const {
    return constant_;
}

Primitive make_add(Scalar constant){
    return Primitive::add(constant);
}

Primitive make_square(){
    return Primitive::square();
}

Scalar evaluate(const Primitive& primitive, Scalar x){
    switch (primitive.kind())
    {
    case PrimitiveKind::AddConstant :
        x = add_value(x, primitive.constant());
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

    switch (primitive.kind())
    {
    case PrimitiveKind::AddConstant :
        s = "Add(" + std::to_string(primitive.constant()) + ")";
        break;

    case PrimitiveKind::Square :
        s = "Square";
        break;
    
    default :
        throw std::logic_error("primitive n'a pas de PrimitiveKind.");
    }

    return s;
}

Morphism::Morphism(){}

Morphism::Morphism(const std::vector<Primitive>& steps)
    : steps_(steps) {

}

const std::vector<Primitive>& Morphism::steps() const {
    return steps_;
}

Morphism identity(){
    Morphism m;

    return m;
}

Morphism lift(Primitive primitive){
    std::vector<Primitive> v{primitive};

    Morphism m(v);

    return m;
}

Scalar evaluate(const Morphism& morphism, Scalar x){
    for(const Primitive& f : morphism.steps()){
        x = evaluate(f, x);
    }

    return x;
}

std::string to_string(const Morphism& morphism){
    if(morphism.steps().empty()) return "Id";
    
    std::string s;
    for(size_t i = 0; i < morphism.steps().size() - 1; i++){
        s += to_string(morphism.steps()[i]) + " >> ";
    }
    s += to_string(morphism.steps()[morphism.steps().size() - 1]);
    
    return s;
}

Morphism compose(const Morphism& g, const Morphism& f){
    std::vector<Primitive> v;

    v.reserve(f.steps().size() + g.steps().size());
    v.insert(v.end(), f.steps().begin(), f.steps().end());
    v.insert(v.end(), g.steps().begin(), g.steps().end());

    Morphism morphism(v);

    return morphism;
}

bool same_syntax(const Morphism& a, const Morphism& b){
    if(a.steps().size() != b.steps().size()) return false;

    for(size_t i = 0; i < a.steps().size(); i++){
        if(a.steps()[i].kind() != b.steps()[i].kind()) return false;
        if(a.steps()[i].kind() == PrimitiveKind::AddConstant){
            if(a.steps()[i].constant() != b.steps()[i].constant()) return false;
        }
    }

    return true;
}

Morphism simplify(const Morphism& input){
    std::vector<Primitive> v;
    Scalar acc = 0;

    for(const Primitive& f : input.steps()){
        if(f.kind() == PrimitiveKind::AddConstant && f.constant() == 0){
            continue;
        }
        if(f.kind() == PrimitiveKind::AddConstant){
            acc += f.constant();
            continue;
        }
        if(acc == 0){
            v.push_back(f);
            continue;
        }
        v.push_back(make_add(acc));
        v.push_back(f);
        acc = 0;
    }
    if(acc != 0){
        v.push_back(make_add(acc));
    }

    Morphism m(v);
    
    return m;

}

Values map_values(const Morphism& f, const Values& input){
    Values v;
    v.reserve(input.size());

    for(const Scalar& x : input){
        Scalar s = evaluate(f, x);
        v.push_back(s);
    }

    return v;
}

Values evaluate(const MapPipeline& pipeline, const Values& input){
    Values v = input;

    for(const Morphism& m : pipeline.stages){
        v = map_values(m, v);
    }

    return v;
}

std::string to_string(const MapPipeline& pipeline){
    std::string s;

    if(pipeline.stages.empty()) return "IdList";

    for(size_t i = 0; i < pipeline.stages.size() - 1; i++){
        s += "Map(" + to_string(pipeline.stages[i]) + ") >> ";
    }
    s += "Map(" + to_string(pipeline.stages[pipeline.stages.size() - 1]) + ")";

    return s;
}

MapPipeline fuse_maps(const MapPipeline& input){
    MapPipeline output;

    if(input.stages.empty()) return output;

    Morphism acc = identity();

    for(const Morphism& m : input.stages){
        acc = compose(m , acc);
    }

    output.stages.push_back(acc);

    return output;
}

}
