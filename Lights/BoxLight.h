//
// Created by marti on 25-Sep-21.
//

#ifndef RT_BOXLIGHT_H
#define RT_BOXLIGHT_H


#include "Light.h"
#include "../Helpers.h"
// Area light: returns randomized points in the box's bounds when queried for info
class BoxLight : public Light {
public:
    BoxLight(Color light, Point p1, Point p2) : Light(light){
        if(p1.x < p2.x){
            this->x_min = p1.x;
            this->x_max = p2.x;
        } else {
            this->x_min = p2.x;
            this->x_max = p1.x;
        }

        if(p1.y < p2.y){
            this->y_min = p1.y;
            this->y_max = p2.y;
        } else {
            this->y_min = p2.y;
            this->y_max = p1.y;
        }

        if(p1.z < p2.z){
            this->z_min = p1.z;
            this->z_max = p2.z;
        } else {
            this->z_min = p2.z;
            this->z_max = p1.z;
        }
    }

    Point getDirection(Point from) override {       //FIXME: maybe get rid of this and distance functions
        // returns direction to the center of the box
        double center_x = ((x_max + x_min) / 2.0);
        double center_y = ((y_max + y_min) / 2.0);
        double center_z = ((z_max + z_min) / 2.0);
        return norm(Point(center_x, center_y, center_z) - from);
    }

    double getDistance(Point from) override {   //FIXME: maybe integrate this with some other calculations, so I make less calls in scene
    }

    std::pair<Point, double> getLightHit(Point from) override{
        Point position = randPoint();
        return {norm(position - from), magnitude(position - from)};
    }

private:
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    double z_min;
    double z_max;

    Point randPoint(){
        double x = getRandDouble(x_min, x_max);
        double y = getRandDouble(y_min, y_max);
        double z = getRandDouble(z_min, z_max);
        return {x, y, z};
    }

};


#endif //RT_BOXLIGHT_H
