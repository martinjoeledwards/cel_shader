//
// Created by marti on 08-Sep-21.
//

#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H
#include "fstream"
#include "iostream"
#include "Color.h"
#include "Helpers.h"

// Stores how the material reacts to light:
// contains diffuse, specular components; diff and spec colors: gloss value; transmissive component or bool?
class Material {
public:
    explicit Material(Color diffuse, Color specular, double fac_diff, double fac_refl, double fac_tran,
                      double spec_weight,
                      double jit_refl, double jit_tran, int gloss, double ior) {
        this->diffuse = diffuse;
        this->specular = specular;
        this->fac_diff = fac_diff;
        this->spec_weight = spec_weight;
        this->fac_refl = fac_refl;
        this->fac_tran = fac_tran;
        this->gloss = (gloss * 2) + 1;      // kind of a hack to keep from getting weird values in the glossiness calc.
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

    Color getDiff(double dot, Color light, double u, double v){
        if(u == -1 || v == -1){
            return diffuse * light * dot;
        }
        int u_coord = floor(u * (double)u_dim);
        int v_coord = floor(v * (double)v_dim);
        Color colVal = ppmImage[v_coord][u_coord];
        return colVal * light * dot * (1 - spec_weight);
    }

    // leave for cel shader?
    Color getSpec(double factor, Color light){  //FIXME: this does a bit for spheres...
//        return {0, 0, 0};
        return light * specular * spec_weight * factor;
    }

//    Color getDiffColor(){
//        if(usesTex) return {0, 1, 0};   //error: not using texture.
//        return diffuse;
//    }


    Color getDiffColor(double u, double v){
        if(!usesTex) return diffuse;
        if(u == -1 || v == -1) return {0, 1, 0};

        int u_coord = floor(u * (double)u_dim);
        int v_coord = floor(v * (double)v_dim);
        Color colVal = ppmImage[v_coord][u_coord];
        return colVal * fac_diff; // so that transparent objs don't take much color
    }

    Color getFullDiff(Color light_col, double dot_val, double refl_fac, double u, double v){
        if(dot_val > 0) {
            Color diff = getDiff(dot_val, light_col, u, v);
            Color spec(0, 0, 0);
            if (refl_fac > 0) {
                spec = getSpec(refl_fac, light_col);
            }
            return diff + spec;
        }
        return {0, 0, 0};
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
    bool hasTex(){
        return usesTex;
    }
    void setTexture(const char* filename){
        usesTex = true;
        texture_filename = filename;
        std::string title;
        std::string curr;
        std::string curr2;
        std::string curr3;
        std::string u_dim_string;
        std::string v_dim_string;
        std::string max_col_string;
//        std::cout << "filename is " << filename << "\n";
        std::ifstream ifs (texture_filename);
        if(ifs.is_open()){
            ifs >> title;
            ifs >> u_dim_string;
            ifs >> v_dim_string;
            ifs >> max_col_string;
//            std::cout << title << " " << u_dim_string << v_dim_string << " " << max_col_string << "\n";

            u_dim = stoi(u_dim_string);
            v_dim = stoi(v_dim_string);
            max_col_val = stoi(max_col_string);
//            std::cout << "ints are " << u_dim << ", " << v_dim << std::endl;
//            std::cout << "max col val is " << max_col_val << std::endl;

            ppmImage = new Color *[v_dim];          //set up image double array
            for (int i = 0; i < v_dim; i++) {
                ppmImage[i] = new Color[u_dim];
            }

            int ct = 0;
            for(int i = 0; i < v_dim; i++){
                for(int j = 0; j < u_dim; j++){
                        ifs >> curr;
                        ifs >> curr2;
                        ifs >> curr3;
                    double r = (double)stoi(curr) / (double)max_col_val;
                    double g = (double)stoi(curr2) / (double)max_col_val;
                    double b = (double)stoi(curr3) / (double)max_col_val;

                    Color test = Color(r, g, b);
                    ppmImage[i][j] = test;
                }
            }
            ifs.close();
        } else {
            std::cout << "file didn't open.\n";
            exit(1);
        }
    }
    int getPath(){
        double prob_sum = fac_diff + fac_refl + fac_tran;
        double random = getRandDouble(0, prob_sum);
        if(random < (fac_diff)){
            return 0;
        } else if (random < fac_diff + fac_refl) {
            return 1;
        }
        return 2;
    }

private:
    Color diffuse;
    Color specular;

    double fac_diff;
    double spec_weight;
    double fac_refl;
    double fac_tran;
    double refrac_index;
    int gloss;
    double refl_jit;
    double tran_jit;

    int u_dim;
    int v_dim;
    int max_col_val = 647;
    const char *texture_filename = "";
    bool usesTex = false;
    Color** ppmImage;
};


#endif //RT_MATERIAL_H
