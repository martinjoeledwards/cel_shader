//
// Created by marti on 07-Sep-21.
//

#ifndef RT_OBJECT_H
#define RT_OBJECT_H
#include "../Point.h"
#include "../Ray.h"
#include "../Material.h"

// Base class for all renderable objects.
//FIXME: later, potentially give default material, then attach real mat later?

class Object {
public:
    explicit Object(Material* inMat){
        myMat = inMat;
    }

//    virtual Point get_hit_point(Ray inRay) = 0;     //used for creating new ray
    virtual double get_dist(Ray inRay) = 0;
    virtual Color calc_col(){       //calculate color...used?
        return myMat->getCol();
    }
    virtual Point getSurfNorm(Point hit, Ray inRay) = 0;
    Material* myMat;
protected:

};


#endif //RT_OBJECT_H
