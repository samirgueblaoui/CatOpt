#include "../include/optimiseur.hpp"

Morphism simplify(const Morphism& input){
    Morphism m;
    for(const Primitive& f : input.steps){
        if(f.kind == PrimitiveKind::AddConstant && f.constant == 0){
            continue;
        }
        m.steps.push_back(f);
    }
    Morphism m2;
    Scalar acc = 0;

    for(const Primitive& f : input.steps){
        if(f.kind == PrimitiveKind::AddConstant){
            acc += f.constant;
            continue;
        }
        if(acc == 0){
            m2.steps.push_back(f);
            continue;
        }
        m2.steps.push_back(make_add(acc));
        m2.steps.push_back(f);
        acc = 0;
    }
    if(acc != 0){
        m2.steps.push_back(make_add(acc));
    }
    
    return m2;

}