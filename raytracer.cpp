//
//  Martin Edwards Sept. 2021
//
//

/* TODO:
 *  bounding volumes
 *  median split algorithm
 *
 *  extra: soft specular and diffuse? Powered specular and diffuse?
*/
//bounding box tree

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
#include "SceneMaker.h"
#include <random>
bool rowThreading = false;
//bool rowThreading = true;
const int num_threads = (int) std::thread::hardware_concurrency() - 1;

void renderRow(Camera myCamera, Scene myScene, DataGrid myGrid, int j, int x_dim){
    for (auto i = 0u; i < x_dim; i++) {
        std::vector<Ray> rayList = myCamera.getRayList(i, j);
        myGrid.set(i, j, myScene.getColorRecursiveMulti(rayList) * 255.0);
    }
}


int main() {
//    srand(time(NULL));
    //Welcome statement
    std::cout << "Welcome to Martin's fabulous Ray tracer!" << std::endl;


////    threading stuff
    std::vector<std::thread> threads;



    SceneMaker sceneMaker;
    Scene myScene2 = *sceneMaker.scene1();

    {
//    double aspect_ratio = 1.0 / .75;
//
//    int sampleSubdiv = 4;
//    bool rayJitter = true;
//
//    auto x_dim = 320u;       //default 320
//    auto y_dim = x_dim / aspect_ratio; //240p
//    double fov = 100.0;
//
//
//    Point cam_from(1.5, 1.5, 1.5);
//    Point cam_at(0, 0, 0);
//    Point cam_up(0, 1, 0);
//
//    Camera myCamera(x_dim, aspect_ratio, fov, cam_from, cam_at, cam_up);
//    myCamera.setRayJitter(rayJitter);
//    myCamera.setSampleSubdiv(sampleSubdiv);
    }

    SceneMaker sceneMaker2;
    Camera myCamera = *sceneMaker2.camera1();
    auto x_dim =myCamera.get_x_dim();
    auto y_dim =myCamera.get_y_dim();

    Progress my_prog(y_dim);

    DataGrid myGrid(x_dim, y_dim);

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

            threads.emplace_back(renderRow, myCamera, myScene2, myGrid, j, x_dim);

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
                    myGrid.set(i, j, myScene2.getColorRecursiveMulti(rayList) * 255.0);
                }
            my_prog.print_progress(j);
        }
    }

    my_prog.print_finish();

    myGrid.save_image();

    return 0;
}

