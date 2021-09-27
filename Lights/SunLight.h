//
// Created by marti on 13-Sep-21.
//

#ifndef RT_SUNLIGHT_H
#define RT_SUNLIGHT_H

#include "Light.h"

class SunLight : public Light{
public:
    SunLight(Color light, Point direction) : Light(light){
        light_direction = norm(direction);
    }

    Point getDirection(Point from) override {
//        position.Print();
        return light_direction;
    }

    double getDistance(Point from) override {
        return 99999;
    }

    std::pair<Point, double> getLightHit(Point from) override{
        return {light_direction,99999};
    }
private:
    Point light_direction;

};


#endif //RT_SUNLIGHT_H
