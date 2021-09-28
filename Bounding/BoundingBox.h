//
// Created by marti on 27-Sep-21.
//

#ifndef RT_BOUNDINGBOX_H
#define RT_BOUNDINGBOX_H


//#include "../Objects/Object.h"

//#include "../Point.h"
#include "../Ray.h"

class BoundingBox {
public:

    BoundingBox(){
        x_min = 0;
        y_min = 0;
        z_min = 0;
        x_max = 0;
        y_max = 0;
        z_max = 0;
    }

    BoundingBox(Point mina, Point maxa){
        x_min = mina.x;
        y_min = mina.y;
        z_min = mina.z;
        x_max = maxa.x;
        y_max = maxa.y;
        z_max = maxa.z;
    }

    BoundingBox combine(BoundingBox inBox){
        double new_x_min = min(inBox.x_min, this->x_min);
        double new_y_min = min(inBox.y_min, this->y_min);
        double new_z_min = min(inBox.z_min, this->z_min);

        double new_x_max = max(inBox.x_max, this->x_max);
        double new_y_max = max(inBox.y_max, this->y_max);
        double new_z_max = max(inBox.z_max, this->z_max);

        return {Point(new_x_min, new_y_min, new_z_min), Point(new_x_max, new_y_max, new_z_max)};
    }

    double getSurfArea(){
        double x_len = x_max - x_min;
        double y_len = y_max - y_min;
        double z_len = z_max - z_min;

        return ((x_len * y_len) + (x_len * z_len) + (y_len * z_len));   //actually surface area / 2, you see.
    }

    bool inside(Point point){
        return (point.x > x_min)
               && (point.x < x_max)
               && (point.y > y_min)
               && (point.y < y_max)
               && (point.z > z_min)
               && (point.z < z_max);
    }

    bool rayHit(Ray inRay){

        double x_min_t = (x_min - inRay.getOrigin().x) / inRay.getDir().x ;
        double x_max_t = (x_max - inRay.getOrigin().x) / inRay.getDir().x ;
        if(x_min_t > x_max_t) std::swap(x_min_t, x_max_t);

        double y_min_t = (y_min - inRay.getOrigin().y) / inRay.getDir().y ;
        double y_max_t = (y_max - inRay.getOrigin().y) / inRay.getDir().y ;
        if(y_min_t > y_max_t) std::swap(y_min_t, y_max_t);

        if(y_min_t > x_max_t || y_max_t < x_min_t) return false;   //no intersection

        double min_hit_t = x_min_t;
        double max_hit_t = x_max_t;
        if(y_min_t > x_min_t) min_hit_t = y_min_t;
        if ( y_max_t < x_max_t ) max_hit_t = y_max_t;


        double z_min_t = (z_min - inRay.getOrigin().z) / inRay.getDir().z ;
        double z_max_t = (z_max - inRay.getOrigin().z) / inRay.getDir().z ;

        if(z_min_t > z_max_t) std::swap(z_min_t, z_max_t);
        if(min_hit_t > z_max_t || max_hit_t < z_min_t) return false;

        if (z_min_t > min_hit_t) min_hit_t = z_min_t;
        if (z_max_t < max_hit_t) max_hit_t = z_max_t;

        return (min_hit_t > 0) ;        // fixed for negative intersections
    }

private:
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    double z_min;
    double z_max;

    double min(double a, double b){
        if(a < b) return a;
        return b;
    }
    double max(double a, double b){
        if(a > b) return a;
        return b;
    }

};


#endif //RT_BOUNDINGBOX_H
