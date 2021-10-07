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

        this->totalArea = getArea(this->p0, this->p1, this->p2);
//        std::cout << "Point is " << vec0 << " " << vec1 << " " << cross(vec1, vec0) << std::endl;
        normal = norm(cross(vec1, vec0));
        distance = -dot(normal, p1);
//        std::cout << "norm and dist: " << normal << ", " << distance << std::endl;

//        self.vec0 = self.p1 - self.p0   # maybe with the definition of my plane, these can't be switched.
//        self.vec1 = self.p2 - self.p0

        this->bbox = BoundingBox(getBounds().first, getBounds().second);

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
    std::pair<Point, Point> getBounds() override {
        double x_min = getMin(p0.x, p1.x, p2.x);
        double x_max = getMax(p0.x, p1.x, p2.x);
        double y_min = getMin(p0.y, p1.y, p2.y);
        double y_max = getMax(p0.y, p1.y, p2.y);
        double z_min = getMin(p0.z, p1.z, p2.z);
        double z_max = getMax(p0.z, p1.z, p2.z);

        return {Point(x_min, y_min, z_min), Point(x_max, y_max, z_max)};
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

    std::pair<double, double> getUV(Point point) {
        double uArea = getArea(point, p0, p1);
        double vArea = getArea(point, p0, p2);
//        std::cout << "u and v are " << uArea / totalArea << " " << vArea / totalArea << "\n";
        return {uArea / totalArea, vArea / totalArea};
    }

private:
    Point p0;
    Point p1;
    Point p2;
    Point normal;
    double distance;
    double totalArea;

    double getMin(double a, double b, double c){        //TODO: make accept arbitrary number of points, so polygons work.
        if(a < b && a < c) return a;
        if(b < c) return b;
        return c;
    }

    double getMax(double a, double b, double c){        //TODO: make accept arbitrary number of points, so polygons work.
        if(a > b && a > c) return a;
        if(b > c) return b;
        return c;
    }

    double getArea(Point center, Point in1, Point in2){
        in1 = in1 - center;
        in2 = in2 - center;
        double pArea = magnitude(cross((in1), (in2)));
        return  pArea / 2.0;
    }
};


#endif //RT_TRIANGLE_H
