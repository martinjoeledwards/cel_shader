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
                      int gloss, double fac_tran, double ior) {
        this->diffuse = diffuse;
        this->specular = specular;
        this->fac_diff = fac_diff;
        this->fac_spec = fac_spec;
        this->fac_amb = fac_amb;
        this->fac_refl = fac_refl;
        this->fac_tran = fac_tran;
        this->gloss = (gloss * 2) + 1;
        this->refrac_index = ior;
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
};


#endif //RT_MATERIAL_H
