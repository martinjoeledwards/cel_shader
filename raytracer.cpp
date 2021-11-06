//
//  Martin Edwards Sept. 2021
//
//

/* TODO:
 *  create objects for distance and normal maps
 *  generate outlines from distance and normal maps
 *  combine color and outlines for final image
*/
//bounding box tree

#include <iostream>
#include <vector>
#include <thread>
#include "Color.h"
#include "Grids/DataGrid.h"
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
#include "Grids/NormalGrid.h"
#include "Grids/DistGrid.h"
#include <random>
bool multiThreading = false;
//bool multiThreading = true;
const int num_threads = (int) std::thread::hardware_concurrency() - 1;

void renderRow(Scene myScene, DataGrid myGrid, int j, int x_dim){
    for (auto i = 0u; i < x_dim; i++) {
        myGrid.set(i, j, myScene.getColorRecursiveMulti(i, j) * 255.0);
    }
}

void renderRowNormals(Scene myScene, NormalGrid myGrid, int j, int x_dim){
    for (auto i = 0u; i < x_dim; i++) {
        myGrid.set(i, j, myScene.getNormalMulti(i, j) * 255.0);
    }
}


int main() {
    //Welcome statement
    std::cout << "Welcome to Martin's fabulous cel shading ray tracer!" << std::endl;


////    threading stuff
    std::vector<std::thread> threads;


    SceneMaker sceneMaker;
//    Scene myScene = *sceneMaker.scene1();
    Scene myScene = *sceneMaker.scene2();
//    Scene myScene = *sceneMaker.scene3();
//    Scene myScene = *sceneMaker.scene4();


    auto x_dim =myScene.myCamera->get_x_dim();
    auto y_dim =myScene.myCamera->get_y_dim();

    Progress my_prog(y_dim);

    DataGrid myGrid(x_dim, y_dim);
    NormalGrid normGrid(x_dim, y_dim);
    DistGrid distGrid(x_dim, y_dim);

    if (multiThreading) {       //slower for cel shading

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

            threads.emplace_back(renderRow, myScene, myGrid, j, x_dim);
//            threads.emplace_back()

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
                    normGrid.set(i, j, myScene.getNormalMulti(i, j));
                    distGrid.set(i, j, myScene.getDistMulti(i, j));
                    myGrid.set(i, j, myScene.getColorRecursiveMulti(i, j) * 255.0);
                }
            my_prog.print_progress(j);
        }
    }

    normGrid.colorize();
    DataGrid normalMap = DataGrid(normGrid);

    distGrid.detectEdges();
    distGrid.quantize(.5);
    DataGrid finalGrid = DataGrid(distGrid);

//    myGrid.addEdges(distGrid);

    my_prog.print_finish();

    normalMap.save_image();
//    finalGrid.save_image();
//    myGrid.save_image();

    return 0;
}

