//
// Created by marti on 13-Sep-21.
//

#ifndef RT_LIGHT_H
#define RT_LIGHT_H
#include "../Point.h"
#include "../Color.h"

class Light {
public:
    Light(Color inColor){
        color = inColor;
    }
    virtual Point getDirection(Point from) = 0;

    virtual double getDistance(Point from) = 0;

    virtual std::pair<Point, double> getLightHit(Point from) = 0;   //returns direction and distance to light

    Color getColor(){
        return color;
    }
protected:
    Color color;


};


#endif //RT_LIGHT_H
