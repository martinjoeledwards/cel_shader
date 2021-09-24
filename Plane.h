//
// Created by marti on 11-Sep-21.
//

#ifndef RT_PLANE_H
#define RT_PLANE_H


#include "Object.h"

class Plane : public Object {
public:
    Plane(Material *inMat, Point normal, double distance) : Object(inMat){
        this->normal = norm(normal);
        this->distance = distance;
    }

    Point get_hit_point(Ray inRay) override {
        return Point();
    }

    double get_dist(Ray inRay) override {
        double checker = dot(normal, inRay.getDir());
        if(checker == 0.0){
            return -1;
        }
        double t = -(dot(normal, inRay.getOrigin()) - distance) / checker;
        if (t > 0){
            return t;
        }
        return -1;
    }

    Color calc_col() override {
        return Object::calc_col();
    }

    Point getSurfNorm(Point hit, Ray inRay) override {
        if (dot(normal, inRay.getDir()) <= 0){
            return normal;
        } else {
            return normal * -1;
        }
    }

private:
    Point normal;
    double distance;
};


#endif //RT_PLANE_H
