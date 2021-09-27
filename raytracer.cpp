//
//  Martin Edwards Sept. 2021
//
//

/* TODO:
 *  jitter reflection and transmission rays
 *  bounding volumes
 *  median split algorithm
 *
 *  extra: soft specular and diffuse? Powered specular and diffuse?
 */
//  axis-aligned box objects, Area lights and shadow calculations, bounding box tree

// FIXME: renders x-dim oppositely
// TODO: advanced light calculations; self-normalizing direction class?
#include <iostream>
#include <vector>
#include <thread>
#include "Color.h"
#include "DataGrid.h"
#include "Helpers.h"
#include "Point.h"
#include "Ray.h"
#include "Objects/Object.h"
#include "Objects/Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "Scene.h"
#include "Objects/Plane.h"
#include "Objects/Triangle.h"
#include "Lights/PointLight.h"
#include "Objects/AABox.h"
#include "Lights/BoxLight.h"
#include <random>
bool rowThreading = false;
const int num_threads = (int) std::thread::hardware_concurrency() - 1;

void renderRow(Camera myCamera, Scene myScene, DataGrid myGrid, int j, int x_dim){
    for (auto i = 0u; i < x_dim; i++) {
        std::vector<Ray> rayList = myCamera.getRayList(i, j);
        myGrid.set(i, j, myScene.getColorRecursiveMulti(rayList) * 255.0);
    }
}


int main() {
    srand(time(NULL));
    //Welcome statement
    std::cout << "Welcome to Martin's fabulous Ray tracer!" << std::endl;

    double aspect_ratio = 1.0 / .75;

    int sampleSubdiv = 1;
    int num_bounces = 2;
    int shadowSamples = 2;
    bool rayJitter = true;

    auto x_dim = 320u;       //default 320
    auto y_dim = x_dim / aspect_ratio; //240p
    double fov = 100.0;

//    threading stuff
    std::vector<std::thread> threads;


    Point cam_from(1, 1, 1);
    Point cam_at(0, 0, 0);
    Point cam_up(0, 1, 0);

    Camera myCamera(x_dim, aspect_ratio, fov, cam_from, cam_at, cam_up);
    myCamera.setRayJitter(rayJitter);
    myCamera.setSampleSubdiv(sampleSubdiv);
//    myCamera.setShadowSamples

    DataGrid myGrid(x_dim, y_dim);

//    create colors
    Color red(1, 0, 0);
    Color green(0, 1, 0);
    Color blue(.3, .3, 1);
    Color white(1, 1, 1);
    Color whiteOut(6, 6, 6);

//    create materials
    Material reflect(white, white, .1, .1, .1, .7, 10, 0, 0);
    Material red_mat(red, red, .1, .3, .1, .7, 6, 0, 0);
    Material green_mat(green, white, .4, .5, .1, 0, 6, 0, 0);
    Material blue_mat(blue, white, .3, .5, .3, 0, 6, 0, 0);
    Material refractive(white, white, .005, .5, .002, .1, 16, .8, 1.3);

//    create objects
    Sphere refSphere(&refractive, Point(-.51, 0.3, -0.3), .5);
    Sphere blueSphere(&reflect, Point(-.5, 0, .6), .3);
    Sphere greenSphere(&green_mat, Point(-.4, 0, -1.2), .3);
    AABox blueBox(&blue_mat, Point(.2, .2, .2), Point(0, 0, 0));

    Point p0(100, 100, -.4);
    Point p1(-100, 100, -.4);
    Point p2(0, -100, 3);
    Triangle triangle(&green_mat, p0, p2, p1);

    Plane reflect_plane(&reflect, Point(1, 1, .3), -.4);
    Plane diff_plane(&green_mat, Point(0, 1, 0), -.2);

//    create lights
    SunLight sunlight(whiteOut, Point(-1, 1, 1));
    PointLight pointlight(white, Point(-1, .5, 0));
    PointLight pointlightup(blue, Point(0, 2, 0));
    BoxLight boxLight(white, Point(-.5, 2, -.5), Point(.2, 3, .2));

    Scene myScene;
    myScene.setNumBounces(num_bounces);
    myScene.setShadowSamples(shadowSamples);

// Add objects
//    myScene.AddObjects(&reflect_plane);
    myScene.AddObjects(&blueSphere);
    myScene.AddObjects(&triangle);
//    myScene.AddObjects(&greenSphere);
    myScene.AddObjects(&refSphere);
    myScene.AddObjects(&diff_plane);
    myScene.AddObjects(&blueBox);       //addobjects needs to be adjusted to use bounding boxes

    // Add lights
//    myScene.AddLights(&pointlight);
//    myScene.AddLights(&sunlight);
//    myScene.AddLights(&pointlightup);
    myScene.AddLights(&boxLight);

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

            threads.emplace_back(renderRow, myCamera, myScene, myGrid, j, x_dim);

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
                    std::vector<Ray> rayList = myCamera.getRayList(i, j);
                    myGrid.set(i, j, myScene.getColorRecursiveMulti(rayList) * 255.0);
                }
            my_prog.print_progress(j);
        }
    }

    my_prog.print_finish();

    myGrid.save_image();

    return 0;
}

