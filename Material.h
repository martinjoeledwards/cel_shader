//
// Created by marti on 08-Sep-21.
//

#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H
#include "fstream"
#include "iostream"
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

    Color getDiff(double dot, Color light, double u, double v){
            int u_coord = floor(u * (double)u_dim);
            int v_coord = floor(v * (double)v_dim);
            Color colVal = ppmImage[u_coord][v_coord];
//            std::cout << "col coord is " << u_coord << ", " << v_coord << std::endl;
//            std::cout << std::endl << "color val is ";
//            colVal.Print();
//            std::cout << std::endl;
            return colVal * light * dot * fac_diff;
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
    bool hasTex(){
        return usesTex;
    }
    void setTexture(const char* filename){      //TODO: read file into some kind of data structure
        usesTex = true;
        texture_filename = filename;
        std::string title;
        std::string curr;
        std::string curr2;
        std::string curr3;
        std::string u_dim_string;
        std::string v_dim_string;
        std::string max_col_string;
        std::cout << "filename is " << filename << "\n";
        std::ifstream ifs (texture_filename);
        if(ifs.is_open()){
//            while(ifs >> title){
//                std::cout << title << "\n";
//            }
//            getline(ifs, title);
//            getline(ifs, u_dim_string);
//            getline(ifs, v_dim_string);
//            ifs >> u_dim_string;
//            ifs >> v_dim_string;
//            getline(ifs, max_col_string);
            ifs >> title;
            ifs >> u_dim_string;
            ifs >> v_dim_string;
            ifs >> max_col_string;
            std::cout << title << " " << u_dim_string << v_dim_string << " " << max_col_string << "\n";

            u_dim = stoi(u_dim_string);
            v_dim = stoi(v_dim_string);
            max_col_val = stoi(max_col_string);
            std::cout << "ints are " << u_dim << ", " << v_dim << std::endl;
            std::cout << "max col val is " << max_col_val << std::endl;

            ppmImage = new Color *[u_dim];          //set up image double array
            for (int i = 0; i < u_dim; i++) {
                ppmImage[i] = new Color[v_dim];
            }
//            std::cout << "got here " << max_col_val << std::endl;

            int ct = 0;
            for(int i = 0; i < v_dim; i++){
                for(int j = 0; j < u_dim; j++){
                    ct++;
//                    getline(ifs, curr);
//                    getline(ifs, curr2);
//                    getline(ifs, curr3);
                        ifs >> curr;
                        ifs >> curr2;
                        ifs >> curr3;
                    double r = (double)stoi(curr) / (double)max_col_val;
                    double g = (double)stoi(curr2) / (double)max_col_val;
                    double b = (double)stoi(curr3) / (double)max_col_val;

//                    double r = (double)stoi(curr);
//                    double g = (double)stoi(curr2);
//                    double b = (double)stoi(curr3);
                    Color test = Color(r, g, b);
//                    std::cout << "curr color is " << std::endl;
//                    test.Print();
                    ppmImage[i][j] = test;
//                    if(rand() / RAND_MAX < .001){
//                        diffuse = test;
//                    }
//                    exit(1);
                }
            }
            std::cout << "pixels: " << ct << std::endl;
//            std::cout << "diffuse val is ";
//            diffuse.Print();


            ifs.close();
        } else {
            std::cout << "file didn't open.\n";
            exit(1);
        }
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

    int u_dim;
    int v_dim;
    int max_col_val = 647;
    const char *texture_filename = "";
    bool usesTex = false;
    Color** ppmImage;
};


#endif //RT_MATERIAL_H
