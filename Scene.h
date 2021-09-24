//
// Created by marti on 08-Sep-21.
//

#ifndef RT_SCENE_H
#define RT_SCENE_H
#include <vector>
#include "Object.h"
#include "SunLight.h"
#include "Light.h"
#include "Color.h"
#include <cmath>
#include <cstdarg>


// Contains objects, ambient light, and point lights.
// TODO: contain global light object?
// FIXME: background color is messy. Use some kind of sphere illumination object instead, I think
// TODO: use variadic functions to add objects and lights

class Scene {
public:
     Scene(Color ambient = Color(.5, .5, .5),
          Color background = Color(.3, .3, .3)){
        this->ambient = ambient;
        this->background = background;
        numBounces = 0;
    }
    void setNumBounces(int num){
         numBounces = num;
     }

    void AddObjects(Object* t){
        this->objects.push_back(t);
    }

    template<typename... Args>
    void AddObjects(Object* t, Args... args){
         this->objects.push_back(t);
        AddObjects(args...);
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

     Color getColorRecursive(Ray inRay){        //Deprecated
//         Point incident(1, -1, 0);
//         Point normal(0, 1, 0);
//         std::cout << "angle is " << get_incidence_angle(incident, normal) << std::endl;

         Color aggregate = GetPixelColor(inRay, numBounces);
         if(aggregate == miss)
             return background;
//         aggregate /= (double)lights.size();    // using clipping now, but this could work better.
         return aggregate.clip();
     }

    Color getColorRecursiveMulti(std::vector<Ray> inRays){
        Color total;
        for(Ray curr_ray : inRays) {
            Color aggregate = GetPixelColor(curr_ray, numBounces);
//            if (aggregate == miss) {
//                total += background;        //refactor to not use background here?
//            } else {
                total += aggregate;
//            }
        }
        total /= (double)inRays.size();
        return total.clip();
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

//            Light *currLight = lights[0];
            Point direction = curr_light->getDirection(hit_point);
            Object* shadowObj = getClosestObject(new_orig, curr_light->getDirection(new_orig)).first;
            double shadowObjDist = getClosestObject(new_orig, curr_light->getDirection(new_orig)).second;
            double light_dist = curr_light->getDistance(new_orig);

            if (shadowObj != nullptr && light_dist > shadowObjDist) {  //if  shadowed, don't compute. Else:
            } else {
                //diffuse
                double dot_val = dot(direction, surf_norm);
                Color diff_col = closest->myMat->getDiff(dot_val, curr_light->getColor());
                if (dot_val > 0)
                    all_light += diff_col;

                //specular      //FIXME: gloss only works with odd values. Artifacts appear with calculations.  //FIXME: use reflection func here
                Point refl_vec = (surf_norm * dot(direction, surf_norm) * 2) - direction;  //get reflection direction
                double refl_fac = pow(dot(direction, refl_vec), currMat.getGloss());        //should be odd multiple?
                if (refl_fac > 0 && dot(direction, surf_norm) > 0) {      //feels hacky, but works...
                    Color spec = currMat.getSpec(refl_fac, Color(1, 1, 1));
                    all_light += spec;
                }
            }
        }
//        reflection
        if(iters > 0 && currMat.getReflFac() > 0.01){   //if surface is reflective and reflections aren't maxed out
            Point reflected_vec = reflect_vector(inRay.getDir(), surf_norm);
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
            Ray refracted_ray(hit_point + (refracted_vec * shift_amount), refracted_vec);
            Color refracted_color = GetPixelColor(refracted_ray, iters);        //FIXME infinite recursion?
            if (refracted_color == miss) {
                all_light += background * currMat.getTranFac(); //this actually really helps...
            } else {
                all_light = all_light + (refracted_color * currMat.getTranFac());
            }
        }

        // end light collection loop

//        return all_light /= (double)lights.size();
        return all_light;
    }

private:
    Color ambient;
    Color background;
    Color miss{-1, -1, -1};
    int numBounces;
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    double shift_amount = 0.0001;
};


#endif //RT_SCENE_H
