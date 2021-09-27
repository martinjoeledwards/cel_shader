//
// Created by marti on 11-Sep-21.
//

#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H
#include <cmath>

#include "Object.h"
//FIXME: store points in array/vector?
//FIXME: normal might be weird...

class Triangle : public Object {
public:
    Triangle(Material *inMat, Point p0, Point p1, Point p2) : Object(inMat) {
        this->p0 = p0;
        this->p1 = p1;
        this->p2 = p2;

        Point vec0 = p1 - p0;
        Point vec1 = p2 - p0;
//        std::cout << "Point is " << vec0 << " " << vec1 << " " << cross(vec1, vec0) << std::endl;
        normal = norm(cross(vec1, vec0));
        distance = -dot(normal, p1);
//        std::cout << "norm and dist: " << normal << ", " << distance << std::endl;

//        self.vec0 = self.p1 - self.p0   # maybe with the definition of my plane, these can't be switched.
//        self.vec1 = self.p2 - self.p0
    };

//    Point get_hit_point(Ray inRay) override {
//        return Point();
//    }

    bool point_in_tri(Point point) {
        Point posNorm = absVal(normal);
//        posNorm.Print();
        int max = maxIndex(posNorm);
//        std::cout << "maxind " << max << std::endl;
        UVPoint new0 = squish(p0, max);
        UVPoint new1 = squish(p1, max);
        UVPoint new2 = squish(p2, max);
        UVPoint newP = squish(point, max);
//        std::cout << "oldpoints are " << p0 << p1 << p2 << point << std::endl;
//        std::cout << "newpoints are " << new0 << new1 << new2 << newP << std::endl;

        UVPoint edge0 = new1 - new0;
        UVPoint edge1 = new2 - new1;
        UVPoint edge2 = new0 - new2;

        UVPoint c0 = newP - new0;
        UVPoint c1 = newP - new1;
        UVPoint c2 = newP - new2;
//        std::cout << "uvspoints are " << c0 << " " << c1 << " " << c2 << std::endl;

        double sign0 = cross(edge0, c0);
        double sign1 = cross(edge1, c1);
        double sign2 = cross(edge2, c2);
//        std::cout << "signs are " << sign0 << " " << sign1 << " " << sign2 << std::endl;
        if(sign0 > 0 && sign1 > 0 && sign2 > 0) return true;
        if(sign0 < 0 && sign1 < 0 && sign2 < 0) return true;
//        std::cout << "false" << std::endl;
        return false;


//        return false;
    }

    double get_dist(Ray inRay) override {
//        std::cout << "dist checkd" << std::endl;
        double checker = dot(normal, inRay.getDir());
        if(checker == 0.0){
            return -1;
        }
        double t = - (dot(normal, inRay.getOrigin()) + distance) / checker;
        if (t <= 0){
            return -1;
        }

        Point hit_point = inRay.getOrigin() + (inRay.getDir() * t);
//        hit_point.Print();
        if (!point_in_tri(hit_point)){
            return -1;
        }
//        std::cout << "t is " << t <<  std::endl;
        return t;

//        t = - (np.dot(tri.normal, ray_o) + tri.distance) / checker

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
    Point p0;
    Point p1;
    Point p2;
    Point normal;
    double distance;
};


#endif //RT_TRIANGLE_H
