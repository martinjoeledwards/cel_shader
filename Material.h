//
// Created by marti on 08-Sep-21.
//

#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H
#include "Color.h"

// Stores how the material reacts to light:
// contains diffuse, specular, ambient components; diff and spec colors: gloss value; transmissive component or bool?
class Material {
public:
    explicit Material(Color diffuse, Color specular, double fac_diff, double fac_spec, double fac_amb, double fac_refl,
                      double fac_tran, double jit_refl, double jit_tran, int gloss, double ior) {
        this->diffuse = diffuse;
        this->specular = specular;
        this->fac_diff = fac_diff;
        this->fac_spec = fac_spec;
        this->fac_amb = fac_amb;
        this->fac_refl = fac_refl;
        this->fac_tran = fac_tran;
        this->gloss = (gloss * 2) + 1;      // kind of a hack to keep from getting weird values in the glossiness calc. Dunno why it happens.
        this->refrac_index = ior;
        this->refl_jit = jit_refl;
        this->tran_jit = jit_tran;
    }
    Color getCol(){
        return diffuse;
    }

    double getGloss(){
        return gloss;
    }

    Color getAmb(Color ambient){
        return ambient * diffuse * fac_amb;
    }

    Color getDiff(double dot, Color light){
        return diffuse * light * dot * fac_diff;
    }

    Color getSpec(double factor, Color light){
        return light * specular * fac_spec * factor;
    }

    double getReflFac(){
        return fac_refl;
    }

    double getTranFac(){
        return fac_tran;
    }

    double getIOR(){
        return refrac_index;
    }

    double getReflJit() {
        return refl_jit;
    }
    double getTranJit() {
        return tran_jit;
    }

private:
    Color diffuse;
    Color specular;
    double fac_diff;
    double fac_spec;
    double fac_amb;
    double fac_refl;
    double fac_tran;
    double refrac_index;
    int gloss;
    double refl_jit;
    double tran_jit;
};


#endif //RT_MATERIAL_H
