//
// Created by marti on 07-Sep-21.
//

#ifndef RT_SPHERE_H
#define RT_SPHERE_H
#include "Object.h"
#include <cmath>

class Sphere: public Object {
public:
    Sphere(Material *inMat, Point center, double rad) : Object(inMat) {
        this->center = center;
        this->rad = rad;
//        this->myMat = myMat;
    }

    Point get_hit_point(Ray inRay) override {
        return Point(0, 0, 0);
    }

    double get_dist(Ray inRay) override{
        double b = 2.0 * dot(inRay.getDir(), inRay.getOrigin() - center);
        double c = pow(magnitude(inRay.getOrigin() - center), 2.0) - (pow(rad, 2.0));
        double disc = pow(b, 2.0) - (4.0 * c);
//        std::cout << "b is " << b << ", c is " << c << ", disc is " << disc << std::endl;
        double t = -1;
        if (disc <= 0.0){     //misses sphere
            return -1;
        } else{
            t = (-b - sqrt(disc) )/ 2.0;        //I should maybe modify this so that you can be in the sphere
        }
        if (t <= 0){
            t = (-b + sqrt(disc) )/ 2.0;
        }
        if (t <= 0){
            return -1.0;  //behind the camera
        }
//        std::cout << "spphere is " << t << std::endl;
        return t;

    }

    Point getSurfNorm(Point hit, Ray inRay) override {
        return norm(hit - center);
    }

private:
    Point center;
    double rad;
};


#endif //RT_SPHERE_H
