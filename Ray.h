//
// Created by marti on 07-Sep-21.
//

#ifndef RT_RAY_H
#define RT_RAY_H
#include "Color.h"
#include "Point.h"

class Ray {
public:
    Ray(Point startPoint, Point direction){
        this->startPoint = startPoint;
        this->direction = norm(direction);
    }

    void offset(){
       startPoint = startPoint + (direction * offset_amt);
    }

    Point getOrigin(){
        return startPoint;
    }

    Point getDir(){
        return direction;
    }

    friend std::ostream& operator<<(std::ostream& os, const Ray& p);


private:
    Point startPoint;
    Point direction;
    const double offset_amt = 0.001;
};

std::ostream& operator<<(std::ostream& os, const Ray& p);

#endif //RT_RAY_H
