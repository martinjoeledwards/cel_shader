//
//  Martin Edwards Sept. 2021
//
//

//testing: pushing variadics branch

// TODO:  variadic parameters; advanced light calculations; self-normalizing direction class?
#include <iostream>
#include <vector>
#include <thread>
#include "Color.h"
#include "DataGrid.h"
#include "Helpers.h"
#include "Point.h"
#include "Ray.h"
#include "Object.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "Scene.h"
#include "Plane.h"
#include "Triangle.h"
#include "PointLight.h"

bool rowThreading = false;
const int num_threads = (int) std::thread::hardware_concurrency() - 1;

void renderRow(Camera myCamera, Scene myScene, DataGrid myGrid, int j, int x_dim, int sample_subdiv){
    for (auto i = 0u; i < x_dim; i++) {
        std::vector<Ray> rayList = myCamera.getRayList(i, j, sample_subdiv);
        myGrid.set(i, j, myScene.getColorRecursiveMulti(rayList) * 255.0);
    }
}


int main() {
    //Welcome statement
    std::cout << "Welcome to Martin's fabulous Ray tracer!" << std::endl;

    double aspect_ratio = 1.0 / .75;

    int sample_subdiv = 1;
    int num_bounces = 1;

    auto x_dim = 320u;       //default 320
    auto y_dim = x_dim / aspect_ratio; //240p
    double fov = 90.0;

//    threading stuff
    std::vector<std::thread> threads;


    Point cam_from(0, .1, 1);
    Point cam_at(0, 0, 0);
    Point cam_up(0, 1, 0);

    Camera myCamera(x_dim, aspect_ratio, fov, cam_from, cam_at, cam_up);

    DataGrid myGrid(x_dim, y_dim);

//    create colors
    Color red(1, 0, 0);
    Color green(0, 1, 0);
    Color blue(.3, .3, 1);
    Color white(1, 1, 1);
    Color whiteOut(8, 6, 4);

//    create materials
    Material reflect(white, white, .1, .1, .1, .7, 10, 0, 0);
    Material red_mat(red, red, .1, .2, .1, .6, 6, 0, 0);
    Material green_mat(green, white, .4, .5, .1, 0, 6, 0, 0);
    Material blue_mat(blue, white, .2, .5, .1, .2, 6, 0, 0);
    Material refractive(white, white, .005, .5, .002, .3, 16, .8, 1.3);

//    create objects
    Sphere redSphere(&refractive, Point(-.51, 0.0, -0.3), .5);
    Sphere blueSphere(&blue_mat, Point(.4, -.4, -0.3), .4);
    Sphere greenSphere(&green_mat, Point(-.4, 0, -1.2), .3);

    Point p0(0, 0, -4);
    Point p1(-2, 3, -5);
    Point p2(-5, 0, -4);
    Triangle triangle(&red_mat, p0, p2, p1);

    Plane reflect_plane(&reflect, Point(0, 1, 0), -.5);

//    create lights
    SunLight sunlight(whiteOut, Point(1, 0, 1));
    PointLight pointlight(white, Point(-1, .5, 0));
    PointLight pointlightup(blue, Point(0, 2, 0));


    Scene myScene;
    myScene.setNumBounces(num_bounces);


    //Add objects
    myScene.AddObject(&reflect_plane);
    myScene.AddObject(&triangle);
    myScene.AddObject(&redSphere);
    myScene.AddObject(&blueSphere);
    myScene.AddObject(&greenSphere);

    // Add lights
    myScene.AddLight(&sunlight);
    myScene.AddLight(&pointlight);
    myScene.AddLight(&pointlightup);

    Progress my_prog(y_dim);


    if (rowThreading) {

//    main loop for everything
        for (auto j = 0u; j < y_dim; j++) {

            while (threads.size() > num_threads - 1) {
                // remove finished threads
                for (int iter = 0; iter < num_threads; iter++) {
                    if (threads[iter].joinable()) {
                        threads[iter].join();
                        threads.erase(threads.begin() + iter);
                        break;
                    }
                }
            }

            threads.push_back(std::thread(renderRow, myCamera, myScene, myGrid, j, x_dim, sample_subdiv));

            my_prog.print_progress(j);
        }

        for (std::thread &t: threads) {
            if (t.joinable()) {
                t.join();
            }
        }

    } else {
        for (auto j = 0u; j < y_dim; j++) {
                for (auto i = 0u; i < x_dim; i++) {
                    std::vector<Ray> rayList = myCamera.getRayList(i, j, sample_subdiv);
                    myGrid.set(i, j, myScene.getColorRecursiveMulti(rayList) * 255.0);
                }
            my_prog.print_progress(j);
        }
    }

    my_prog.print_finish();

    myGrid.save_image();

    return 0;
}

