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
    double getShadowFactor(Point in_point, Light *in_light){
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

    Color GetPixelColor(Ray inRay, int iters){      //FIXME: use spherical stuff for diffuse color

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

        int prob_sum = currMat.getPath();

        if(prob_sum == 0) {
            double u = -1;
            double v = -1;
            if (closest->myMat->hasTex()) {
                closest->setUV(hit_point, &u, &v);
            }

            Color all_light(0, 0, 0);

            Color diff_spec = Color(0, 0, 0);
            //calculate direct illumination
            for (auto curr_light: lights) {

                double shadowFactor = getShadowFactor(float_point, curr_light);

                Color diff_col_looped(0);
                for(int k = 0; k < myCamera->getShadowSamples(); k++ ){   // loop here for jittered samples!
                    Point direction = curr_light->getDirection(hit_point);

                    Point refl_vec =
                            (surf_norm * dot(direction, surf_norm) * 2) - direction;  //get reflection direction
                    double refl_fac = pow(dot(direction, refl_vec), currMat.getGloss());

                    double dot_val = dot(direction, surf_norm);

                    diff_col_looped += closest->myMat->getFullDiff(curr_light->getColor(), dot_val, refl_fac, u, v);
                }
                diff_col_looped /= myCamera->getShadowSamples();

                all_light += diff_col_looped * Color(1 - shadowFactor);
            }

            //calculate indirect illumination
            Color indirect(0, 0, 0);
            if(iters > 0) {
                for (int i = 0; i < myCamera->getIndirectSamples(); i++) {
                    Point newDir = randNormalJitter(surf_norm);
                    Ray indirect_ray(hit_point + (newDir * shift_amount), newDir);
                    Color indirect_col = GetPixelColor(indirect_ray, 0);    //only allow 1 bounce for now

//                    indirect += indirect_col * dot(surf_norm, newDir); // takes angle into account
                    indirect += indirect_col; // doesn't.
                }
            }
            indirect /= myCamera->getIndirectSamples();
            all_light +=  indirect * currMat.getDiffColor(u, v);

            return all_light;
        }

        else if (prob_sum == 1) {
            if(iters > 0){     // if bounces aren't maxed out
                Point reflected_vec = reflect_vector(inRay.getDir(), surf_norm);
                reflected_vec = jitter_vector(reflected_vec, currMat.getReflJit());
                Ray reflected_ray(hit_point + (reflected_vec * shift_amount), reflected_vec);

                Color reflected_color = GetPixelColor(reflected_ray, iters - 1);
                return reflected_color;
            }
            return Color(0);
        }

        else if (prob_sum == 2) {
//        refraction
            if (iters > 0){
                Point refracted_vec = refract_vec(currMat.getIOR(), inRay.getDir(), surf_norm);
                refracted_vec = jitter_vector(refracted_vec, currMat.getTranJit());
                Ray refracted_ray(hit_point + (refracted_vec * shift_amount), refracted_vec);
                Color refracted_color = GetPixelColor(refracted_ray,iters - 1);  //FIXME fixed infinite recursion, I guess
                return refracted_color;
            }
            return Color(0);    // return no light calculated
        }
        std::cout << "\nprob sum is " << prob_sum << std::endl;
        std::cout << "error in material: no light component calculated" << std::endl;
        exit(1);
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
