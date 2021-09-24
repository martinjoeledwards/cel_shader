//
// Created by marti on 08-Sep-21.
//

#ifndef RT_CAMERA_H
#define RT_CAMERA_H
#include "Point.h"
#include <cmath>
#include <vector>
#include "Ray.h"

//TODO: clean up this class. There's unnecessary stuff.
// Takes in location, aspect ratio, etc. Returns rays when queried with i and j.
//TODO: Multi-sampling: return a list of rays

class Camera {
public:
    Camera(unsigned int x_dim, double aspect, double fov, Point from, Point at, Point up){
        this->x_dim = x_dim;
        this->aspect = aspect;
        this->fov = fov;
        this->from = from;
        this->at = at;
        this->up = up;

        this->y_dim = this->x_dim / aspect;
        N = norm(sub(from, at));
        U = norm(cross(up, N));
        V = norm(cross(N, U));

        grid_center = this->from - N;

        dist_x = (tan(PI * (fov / 2.0) / 180.0));
        dist_y = dist_x / aspect;

        incr = (2 * dist_x) / (x_dim - 1);  //account for loops not going "all the way"
        top_right = top_right + (U * dist_x) + (V * dist_y);

        u_step_vec = U.negate() * incr;
        v_step_vec = V.negate() * incr;
    }

    Ray getRay(unsigned int i, unsigned int j){
        Point grid_point = top_right + (u_step_vec * i) + (v_step_vec * j);
        return Ray(from, grid_point - from);
    }

    std::vector<Ray> getRayList(unsigned int i, unsigned int j, int sample_subdiv){
        std::vector<Ray> rayList;
//        auto i_step = (double)i;
//        auto j_step = (double)j;
        Point pixel_center = top_right + (u_step_vec * i) + (v_step_vec * j);
        Point u_subdiv_step = u_step_vec / (sample_subdiv );
        Point v_subdiv_step = v_step_vec / (sample_subdiv );
        Point pixel_start = pixel_center + (u_subdiv_step / 2) + (v_subdiv_step / 2);
        for(int f = 0; f < sample_subdiv; f++){
            for(int g = 0; g < sample_subdiv; g++){

                Point grid_point = pixel_start + u_subdiv_step * f + v_subdiv_step * g;
//                Point grid_point = top_right + (u_step_vec * i) + (v_step_vec * j)
//                        + u_subdiv_step * f + v_subdiv_step * g;
                rayList.push_back(Ray(from, grid_point - from));
            }
        }
//        std::cout << "size of list is " << rayList.size() << std::endl;
        return rayList;
//        Point grid_point = top_right + (u_step_vec * i) + (v_step_vec * j);
//        return Ray(from, grid_point - from);
    }

private:
    double PI = 3.1415926;
    unsigned int x_dim;
    unsigned int y_dim;
    double aspect;
    double fov;
    Point from;
    Point at;
    Point up;

    Point N;
    Point U;
    Point V;

    Point grid_center;

    double dist_x;
    double dist_y;
    double incr;

    Point top_right;
    Point u_step_vec;
    Point v_step_vec;
};


#endif //RT_CAMERA_H
