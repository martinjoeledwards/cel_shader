//
// Created by marti on 11-Sep-21.
//

#ifndef RT_UVPOINT_H
#define RT_UVPOINT_H
#include <iostream>

struct UVPoint {
    UVPoint(double u = 0, double v = 0){
        this->u = u;
        this->v = v;
    }

    UVPoint operator- ( UVPoint& p) {
        UVPoint point;
        point.u = this->u - p.u;
        point.v = this->v - p.v;
        return point;
    }

    void Print(){
        std::cout << "uvpoint is (" << this->u << ',' << this->v << ")" << std::endl;
    }

    double u;
    double v;
};

double cross(UVPoint fir, UVPoint sec);

std::ostream& operator<<(std::ostream& os, const UVPoint& p);


#endif //RT_UVPOINT_H
