//
// Created by marti on 08-Sep-21.
//

#ifndef RT_SCENE_H
#define RT_SCENE_H
#include <vector>
#include "Objects/Object.h"
#include "Lights/SunLight.h"
#include "Lights/Light.h"
#include "Color.h"
#include "Point.h"
#include "Bounding/Node.h"
#include "Camera.h"
#include <cmath>
#include <cstdarg>


// Contains objects, ambient light, and point lights.
// TODO: contain global light object?
// FIXME: background color is crufty. Use some kind of sphere illumination object instead, I think


class Scene {
public:
     Scene(Color ambient = Color(.5, .5, .5),
          Color background = Color(.3, .3, .3)){
         this->ambient = ambient;
         this->background = background;
    }

    void AddObjects(Object* t){
        this->objects.push_back(t);

        rootNode.addObject(t);
    }

    template<typename... Args>
    void AddObjects(Object* t, Args... args){
         this->objects.push_back(t);
        AddObjects(args...);

        rootNode.addObject(t);
     }

     void initialize(){
         rootNode.initialize();
     }

    void AddLights(Light* t){
        this->lights.push_back(t);
    }

    template<typename... Args>
    void AddLights(Light* t, Args... args){
        this->lights.push_back(t);
        AddLights(args...);
    }

    std::pair<Object*, double> getClosestObject(Point ray_start, Point ray_dir){
        double lowest_t = 99999;
        Ray shoot(ray_start, ray_dir);
        int lowest_key = -1;
        int size = (int)objects.size();
        for(int i = 0; i < size; i++){
            double dist = objects[i]->get_dist(shoot);
            if(dist < lowest_t && dist != -1){
                lowest_t = dist;
                lowest_key = i;
//                std::cout << "i is " << i << std::endl;
            }
        }
        if(lowest_key == -1){
            return {NULL, -1};
        }
        return {objects[lowest_key], lowest_t};
     }

     // Returns 1 if completely in shadow. Returns 0 if completely lit.
     double getShadowFactor(Point new_orig, Light *in_light){   //TODO: if light is not area light, only do 1 sample.
         double shadowFac = 0;
         for (int i = 0; i < myCamera->getShadowSamples(); i++) {
             auto light_hit = in_light->getLightHit(new_orig);

             auto shadow = getClosestObject(new_orig, light_hit.first);
             Object *shadowObj = shadow.first;
             double shadowObjDist = shadow.second;

             if (shadowObj != nullptr && light_hit.second > shadowObjDist) {
                 shadowFac += 1;
             }
         }
//         std::cout << shadowFac << "\n";
         return shadowFac / myCamera->getShadowSamples();
     }

    Color getColorRecursiveMulti(std::vector<Ray> inRays){
        Color total;
        for(Ray curr_ray : inRays) {
            Color aggregate = GetPixelColor(curr_ray, myCamera->getNumBounces());
                total += aggregate;
        }
        total /= (double)inRays.size();     // averages light sample colors
        return total.clip();        // clips to light value of 1
    }

    Color GetPixelColor(Ray inRay, int iters){
        double lowest_t;

        auto myPair = getClosestObject(inRay.getOrigin(), inRay.getDir());
        Object* closest = myPair.first;
        lowest_t = myPair.second;
        if(!closest){       //no object hit: return miss
            return background;
        }

        Point hit_point = inRay.getOrigin() + inRay.getDir() * lowest_t;
        Point surf_norm = closest->getSurfNorm(hit_point, inRay);
        Point new_orig = hit_point + (surf_norm * shift_amount); //FIXME: offset new ray in direction of new ray, not normal


        Color all_light(0, 0, 0);
        Material currMat = *(closest->myMat);


//        ambient
        Color ambient_col = currMat.getAmb(ambient);
        all_light += ambient_col;

        //begin light calculation loop per light
        for(auto curr_light : lights) {

//            Fixme: for now, I'm keeping the diffuse and specular colors coming from the center of the light. No softness yet. Can do later.
            double shadowFactor = getShadowFactor(new_orig, curr_light);
            Point direction = curr_light->getDirection(new_orig);

            //diffuse

            Color diff_col;
            double dot_val = dot(direction, surf_norm);
            if(closest->myMat->hasTex()) {
                auto u_v = closest->getUV(hit_point);

                double u = u_v.first;
                double v = u_v.second;
//                std::cout << "u and v: " << u << "," << v << std::endl;
                diff_col = closest->myMat->getDiff(dot_val, curr_light->getColor(), u, v);
            } else {
                diff_col = closest->myMat->getDiff(dot_val, curr_light->getColor());
            }


            if (dot_val < 0)
                diff_col = Color(0, 0, 0);
            all_light += diff_col * Color(1 - shadowFactor);

            //specular
            Point refl_vec = (surf_norm * dot(direction, surf_norm) * 2) - direction;  //get reflection direction
            double refl_fac = pow(dot(direction, refl_vec), currMat.getGloss());
            Color spec_col;
            if (refl_fac > 0 && dot(direction, surf_norm) > 0) {
                spec_col = currMat.getSpec(refl_fac, Color(1, 1, 1));
            } else {
                spec_col = Color(0, 0, 0);
            }
            all_light += spec_col * Color(1 - shadowFactor);

        }
//        reflection
        if(iters > 0 && currMat.getReflFac() > 0.01){   //if surface is reflective and reflections aren't maxed out
            Point reflected_vec = reflect_vector(inRay.getDir(), surf_norm);
            reflected_vec = jitter_vector(reflected_vec,currMat.getReflJit());
            Ray reflected_ray(hit_point + (reflected_vec * shift_amount), reflected_vec);

            Color reflected_color = GetPixelColor(reflected_ray, iters - 1);
            if (reflected_color == miss) {
                all_light += background * currMat.getReflFac(); //this actually really helps...
            } else {
                all_light = all_light + (reflected_color * currMat.getReflFac());
            }
        }

//        refraction (WIP)
        if(currMat.getTranFac() > 0.01){
            //recursively calculate reflectance color
            Point refracted_vec = refract_vec(currMat.getIOR(), inRay.getDir(), surf_norm);
            refracted_vec = jitter_vector(refracted_vec, currMat.getTranJit());
            Ray refracted_ray(hit_point + (refracted_vec * shift_amount), refracted_vec);
            Color refracted_color = GetPixelColor(refracted_ray, iters);        //FIXME infinite recursion?
            if (refracted_color == miss) {
                all_light += background * currMat.getTranFac(); //this actually really helps...
            } else {
                all_light = all_light + (refracted_color * currMat.getTranFac());
            }
        }

        // end light collection loop

        return all_light;
    }

    void SetCamera(Camera *pCamera){
         delete myCamera;
         myCamera = pCamera;
//         numBounces = myCamera->getNumBounces();
//         shadowSamples = myCamera->getShadowSamples();
     }

    Camera* myCamera = new Camera();

private:
    Color ambient;
    Color background;
    Color miss{-1, -1, -1};
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    double shift_amount = 0.0001;
    Node rootNode;
};


#endif //RT_SCENE_H
