#include "../include/categorie.hpp"

#include <stdexcept>


Scalar identity_value(Scalar x){
    return x;
}

Scalar add_value(Scalar x, Scalar constant){
    return x + constant;
}

Scalar square_value(Scalar x){
    return x * x;
}

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

Morphism compose(const Morphism& g, const Morphism& f){
    Morphism morphism;

    morphism.steps.reserve(f.steps.size() + g.steps.size());

    morphism.steps.insert(morphism.steps.end(),
        f.steps.begin(), f.steps.end());

    morphism.steps.insert(morphism.steps.end(),
        g.steps.begin(), g.steps.end());

    return morphism;
}

bool same_syntax(const Morphism& a, const Morphism& b){
    if(a.steps.size() != b.steps.size()) return false;

    for(size_t i = 0; i < a.steps.size(); i++){
        if(a.steps[i].kind != b.steps[i].kind) return false;
        if(a.steps[i].kind == PrimitiveKind::AddConstant){
            if(a.steps[i].constant != b.steps[i].constant) return false;
        }
    }

    return true;
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