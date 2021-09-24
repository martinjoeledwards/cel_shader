//
// Created by marti on 11-Sep-21.
//

#include "UVPoint.h"

double cross(UVPoint fir, UVPoint sec) {
    return (fir.u * sec.v - fir.v * sec.u);
}

std::ostream& operator<<(std::ostream& os, const UVPoint& p){
    os << "(" << p.u << ',' << p.v << ") ";
    return os;
}