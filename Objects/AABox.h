//
// Created by marti on 25-Sep-21.
//

#ifndef RT_AABOX_H
#define RT_AABOX_H


#include "Object.h"

class AABox: public Object {
public:
    AABox(Material *inMat, Point minPt, Point maxPt) : Object(inMat) {      //FIXME: make sure mins and maxes are right

        x_min = minPt.x;
        x_max = maxPt.x;
        y_min = minPt.y;
        y_max = maxPt.y;
        z_min = minPt.z;
        z_max = maxPt.z;

        if(x_min > x_max) std::swap(x_min, x_max);
        if(y_min > y_max) std::swap(y_min, y_max);
        if(z_min > z_max) std::swap(z_min, z_max);

        this->bbox = BoundingBox(getBounds().first, getBounds().second);

    }

    double get_dist(Ray inRay) override {   //FIXME: returns rays from behind the camera if inside the box
        double x_min_t = (x_min - inRay.getOrigin().x) / inRay.getDir().x ;
        double x_max_t = (x_max - inRay.getOrigin().x) / inRay.getDir().x ;
        if(x_min_t > x_max_t) std::swap(x_min_t, x_max_t);

        double y_min_t = (y_min - inRay.getOrigin().y) / inRay.getDir().y ;
        double y_max_t = (y_max - inRay.getOrigin().y) / inRay.getDir().y ;
        if(y_min_t > y_max_t) std::swap(y_min_t, y_max_t);

        if(y_min_t > x_max_t || y_max_t < x_min_t) return -1;   //no intersection

        double min_hit_t = x_min_t;
        double max_hit_t = x_max_t;
        if(y_min_t > x_min_t) min_hit_t = y_min_t;
        if ( y_max_t < x_max_t ) max_hit_t = y_max_t;


        double z_min_t = (z_min - inRay.getOrigin().z) / inRay.getDir().z ;
        double z_max_t = (z_max - inRay.getOrigin().z) / inRay.getDir().z ;

        if(z_min_t > z_max_t) std::swap(z_min_t, z_max_t);
        if(min_hit_t > z_max_t || max_hit_t < z_min_t) return -1;

        if (z_min_t > min_hit_t) min_hit_t = z_min_t;
        if (z_max_t < max_hit_t) max_hit_t = z_max_t;

        if (min_hit_t > 0) return min_hit_t;        // fixed for negative intersections
        return -1;



    }

    Point getSurfNorm(Point hit, Ray inRay) override {
        if(isClose(hit.x, x_min)) return {-1, 0, 0};
        if(isClose(hit.x, x_max)) return {1, 0, 0};
        if(isClose(hit.y, y_min)) return {0, -1, 0};
        if(isClose(hit.y, y_max)) return {0, 1, 0};
        if(isClose(hit.z, z_min)) return {0, 0, -1};
        if(isClose(hit.z, z_max)) return {0, 0, 1};
    }

    std::pair<Point, Point> getBounds() override {
        return {Point(x_min, y_min, z_min), Point(x_max, y_max, z_max)};
    }

private:
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    double z_min;
    double z_max;

    bool isClose(double a, double b){
        if(std::abs(a - b) < 0.0001) return true;
        return false;
    }
};


#endif //RT_AABOX_H
