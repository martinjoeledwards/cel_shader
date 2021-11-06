//
// Created by marti on 06-Sep-21.
//

#ifndef RT_COLOR_H
#define RT_COLOR_H
#include <string>
#include "Point.h"


struct Color {
    Color(double r, double g, double b){
        this->r = r;
        this->g = g;
        this->b = b;
    }

    explicit Color(double all){
        this->r = all;
        this->g = all;
        this->b = all;
    }

    Color(){
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }

    void Print(){
        std::cout << "(" << r << ", " << g << ", " << b << ")" << std::endl;
    }

    Color operator* (const double d){     //FIXME: these aren't working for some reason
        Color col;
        col.r = this->r * d;
        col.g = this->g * d;
        col.b = this->b * d;
        return col;
    }

//    Color operator* (double d){
//        Color col(1, 1, 1);
////        col.r = r * d;
//
//        return col;
//    }


    Color clip() const{
        Color clipped(this->r, this->g, this->b);
        if(clipped.r > 1) clipped.r = 1;
        if(clipped.g > 1) clipped.g = 1;
        if(clipped.b > 1) clipped.b = 1;
        return clipped;
    }

    Color operator* (Color c){      //just changed this
        Color col;
        col.r = this->r * c.r;
        col.g = this->g * c.g;
        col.b = this->b * c.b;
        return col;
    }

    Color operator+ (Color c){
        Color col;
        col.r = this->r + c.r;
        col.g = this->g + c.g;
        col.b = this->b + c.b;
        return col;
    }

    Color operator+= (Color c){
        this->r += c.r;
        this->g += c.g;
        this->b += c.b;
//        return *this; //needed?
    }

    Color operator/= (double d){    //this couldn't be a reference... weird.
        this->r /= d;
        this->g /= d;
        this->b /= d;
        return *this; //needed?
    }

    Color operator= (Color c){
        this->r = c.r;
        this->g = c.g;
        this->b = c.b;
        return *this;
    }

    bool operator== (Color c){    //this couldn't be a reference... weird.
        return (this->r == c.r && this->g == c.g && this->b == c.b);
    }

    double r;
    double g;
    double b;

    void normalize() {
        double r2 = r * r;
        double g2 = g * g;
        double b2 = b * b;
        double sum = r2 + g2 + b2;
        double root = sqrt(sum);
        r = r / root;
        g = g / root;
        b = b / root;

    }
};



#endif //RT_COLOR_H
