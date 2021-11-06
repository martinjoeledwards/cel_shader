//
// Created by marti on 07-Sep-21.
//

#ifndef RT_POINT_H
#define RT_POINT_H
#include <string>
#include <iostream>
#include <math.h>
#include "UVPoint.h"
//#include "Helpers.h"

//supports vectors / points; basically a triple value struct with operations

struct Point {
    Point(double inx = 0, double iny = 0, double inz = 0){
        x = inx;
        y = iny;
        z = inz;
    }

    Point(Point const &inP){
        x = inP.x;
        y = inP.y;
        z = inP.z;
    }

    double x, y, z;

    Point operator+ (const Point& p) {
        Point point;
        point.x = this->x + p.x;
        point.y = this->y + p.y;
        point.z = this->z + p.z;
        return point;
    }

    Point operator+ (const double d) {
        Point point;
        point.x = this->x + d;
        point.y = this->y + d;
        point.z = this->z + d;
        return point;
    }

//    Point operator- ( Point& p) {
//        Point point;
//        point.x = this->x - p.x;
//        point.y = this->y - p.y;
//        point.z = this->z - p.z;
////        std::cout << "sub point is " << this. << std::endl;
//        return point;
//    }

    Point operator- ( Point p) {
        Point point;
        point.x = this->x - p.x;
        point.y = this->y - p.y;
        point.z = this->z - p.z;
//        std::cout << "sub point is " << this. << std::endl;
        return point;
    }

    Point operator* (const double& d){
        Point point;
        point.x = this->x * d;
        point.y = this->y * d;
        point.z = this->z * d;
//        point.Print();
        return point;
    }

    Point operator* (const Point& p){
        Point point;
        point.x = this->x * p.x;
        point.y = this->x * p.y;
        point.z = this->x * p.z;
        return point;
    }

    Point square(){
        Point point;
        point.x = pow(this->x, 2);
        point.y = pow(this->y, 2);
        point.z = pow(this->z, 2);
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;
        return point;
//        this->x *= this->x;
//        this->y *= this->y;
//        this->z *= this->z;
    }

    void root(){
        this->x = sqrt(x);
        this->y = sqrt(y);
        this->z = sqrt(z);
    }

    Point operator/ (const double& d){
        Point point;
        point.x = this->x / d;
        point.y = this->y / d;
        point.z = this->z / d;
        return point;
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& p);

    Point negate(){
        return {-this->x, -this->y, -this->z};
    }

    void Print(){
        std::cout << "point is (" << this->x << ',' << this->y << ',' << this->z << ")" << std::endl;
    }

};

double magnitude(Point inP);

Point norm(Point inP);

Point sub(Point first, Point second);

Point cross(Point fir, Point sec);

Point absVal(Point in);

int maxIndex(Point in);

UVPoint squish(Point in, int index);

double dot(Point fir, Point sec);

Point reflect_vector(Point direction, Point normal);

double get_incidence_angle(Point incident, Point normal);

Point refract_vec(double ior, Point incident, Point normal);

Point jitter_vector(Point vector, double amount);

Point randNormalJitter(Point normal);

std::ostream& operator<<(std::ostream& os, const Point& p);


#endif //RT_POINT_H
