//
// Created by marti on 07-Sep-21.
//

#include "Ray.h"

std::ostream& operator<<(std::ostream& os, const Ray& r){
    os << "(" << r.startPoint << ',' << r.direction << ")";
    return os;
}