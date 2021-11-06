//
// Created by marti on 08-Sep-21.
//

#ifndef RT_SCENE_H
#define RT_SCENE_H
#include <vector>
#include "Objects/Object.h"
#include "Lights/SunLight.h"
#include "Lights/BoxLight.h"
#include "Lights/Light.h"
#include "Color.h"
#include "Point.h"
#include "Bounding/Node.h"
#include "Camera.h"
#include <cmath>
#include <cstdarg>


// Contains objects and lights.
// FIXME: potentially hide the good ol' globe light.
// FIXME: refactor the background factor to actually be a globe light.


class Scene {
public:

    Scene(Color background = Color(.3, .3, .3)) {
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
            }
        }
        if(lowest_key == -1){
            return {NULL, -1};
        }
        return {objects[lowest_key], lowest_t};
    }

    // Returns 1 if completely in shadow. Returns 0 if completely lit.
    double getShadowFactor(Object* closest, Point in_point, Light *in_light){
        if (typeid(BoxLight) == typeid(*in_light)) {   // if boxlight, run multiple samples. Later: extend to area light class
            double shadowFac = 0;
            for (int i = 0; i < myCamera->getShadowSamples(); i++) {
                auto light_hit = in_light->getLightHit(in_point);

                auto shadow = getClosestObject(in_point, light_hit.first);
                Object *shadowObj = shadow.first;
                double shadowObjDist = shadow.second;

                if (shadowObj != nullptr && light_hit.second > shadowObjDist) {
                    shadowFac += 1;
                }
            }
            return shadowFac / myCamera->getShadowSamples();
        } else {
            auto light_hit = in_light->getLightHit(in_point);

            auto shadow = getClosestObject(in_point, light_hit.first);
            Object *shadowObj = shadow.first;
            double shadowObjDist = shadow.second;

            if (shadowObj == closest) return 0;     //no shadow on self objects
            if (shadowObj != nullptr && light_hit.second > shadowObjDist) {
                return 1;
            }
            return 0;
        }
    }

    Color getColorRecursiveMulti(unsigned int i, unsigned int j){
        Color total;
        std::vector<Ray> inRays = myCamera->getRayList(i, j);
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
        Object *closest = myPair.first;
        lowest_t = myPair.second;
        if (!closest) {       //no object hit: return background color
            return background;
        }

        Point hit_point = inRay.getOrigin() + inRay.getDir() * lowest_t;
        Point surf_norm = closest->getSurfNorm(hit_point, inRay);
        Point float_point = hit_point + (surf_norm * shift_amount);

        Material currMat = *(closest->myMat);

        double u = -1;
        double v = -1;
        if (closest->myMat->hasTex()) {
            closest->setUV(hit_point, &u, &v);
        }

        Color all_light(0, 0, 0);

        //calculate direct illumination
        for (auto curr_light: lights) {

            double shadowFactor = getShadowFactor(closest, float_point, curr_light);

            Color diff_col_looped(0);
            Color shadow_color(0);
            for(int k = 0; k < myCamera->getShadowSamples(); k++ ){   // loop here for jittered samples!
                Point direction = curr_light->getDirection(hit_point);

                Point refl_vec =
                        (surf_norm * dot(direction, surf_norm) * 2) - direction;  //get reflection direction
                double refl_fac = pow(dot(direction, refl_vec), currMat.getGloss());

                double dot_val = dot(direction, surf_norm);

//                diff_col_looped += closest->myMat->getFullDiff(curr_light->getColor(), dot_val, refl_fac, u, v);
//                if(dot_val > 0)
                double q_vals[] = {.3, .6, .9};
                int size = 3;
                double olddot = dot_val;
                for(int z = 0; z < size; z++){
                    if (dot_val < q_vals[z]){
                        dot_val = q_vals[z];
                        break;
                    }
                    if(z == size - 1){
                        dot_val = q_vals[z];
                    }
                }


                if(refl_fac < .8) {     //quantize the specular value
                    refl_fac = 0;
                } else {
                    refl_fac = .9;
                }
                double shadow_power = .2;

                shadow_color += closest->myMat->getDiff(shadow_power,curr_light->getColor(), u, v);
                diff_col_looped += closest->myMat->getDiff(dot_val,curr_light->getColor(), u, v);
                diff_col_looped += closest->myMat->getSpec(refl_fac,curr_light->getColor());
            }
            shadow_color /= myCamera->getShadowSamples();
            diff_col_looped /= myCamera->getShadowSamples();

//            diff_col_looped.normalize();      //these produce a washed-out kinda look
//            diff_col_looped = diff_col_looped * 3;

//            all_light += diff_col_looped;
            if(shadowFactor > .5){
                all_light += shadow_color;
            } else {
                all_light += diff_col_looped;
            }
//            all_light += diff_col_looped * Color(1 - shadowFactor);
        }

        return all_light;
    }

    void SetCamera(Camera *pCamera){
        delete myCamera;
        myCamera = pCamera;
    }

    Camera* myCamera = new Camera();

private:
    Color background;
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    double shift_amount = 0.0001;
    Node rootNode;
};


#endif //RT_SCENE_H
