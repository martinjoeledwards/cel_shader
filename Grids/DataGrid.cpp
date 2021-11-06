//
// Created by marti on 06-Sep-21.
//

#include "DataGrid.h"
#include "fstream"
#include "iostream"
#include "../Helpers.h"
#include "../Point.h"

DataGrid::DataGrid(unsigned int in_x_dim, unsigned in_y_dim) {
    x_dim = in_x_dim;
    y_dim = in_y_dim;

    image_grid = new Color *[x_dim];
    for (int i = 0; i < x_dim; i++) {
        image_grid[i] = new Color[y_dim];
    }
}

unsigned int DataGrid::getXDim() {
    return x_dim;
}
unsigned int DataGrid::getYDim() {
    return y_dim;
}

Color DataGrid::get(unsigned int x, unsigned int y) {
    return image_grid[x][y];
}
Color* DataGrid::getRef(unsigned int x, unsigned int y){
    return &image_grid[x][y];
}

void DataGrid::set(unsigned int x, unsigned int y, Color input) {
    image_grid[x][y] = input;
}

void DataGrid::save_image(std::string folder) {

    std::string filename = folder + "/image_" + get_time_string() + ".ppm";

    std::ofstream ofs(filename, std::ios_base::out | std::ios_base::binary);
    ofs << "P6" << std::endl << x_dim << ' ' << y_dim << std::endl << 255 << std::endl;

    for (auto j = 0u; j < y_dim; ++j){
        for (auto i = 0u; i < x_dim; ++i){
            Color val = image_grid[x_dim - i - 1][j];       //Note: this line here fixes the output, so it matches view correctly
            ofs << (char) val.r << (char) val.g << (char) val.b;
        }
    }

    ofs.close();

    std::cout << "Image saved to " <<  filename << std::endl;
}

DataGrid::DataGrid(DistGrid inGrid) {
    x_dim = inGrid.getXDim();
    y_dim = inGrid.getYDim();

    image_grid = new Color *[x_dim];
    for (int i = 0; i < x_dim; i++) {
        image_grid[i] = new Color[y_dim];
//        image_grid[i] = Color(inGrid[])
    }
    for(int i = 0; i < y_dim; i++){
        for (int j = 0; j < x_dim; j++){
            image_grid[j][i] = Color(inGrid.get(j, i));
//            image_grid[j][i] = Color(125);

        }
    }
}

void DataGrid::addEdges(DistGrid grid) {
    for(int i = 0; i < x_dim; i++){
        for (int j = 0; j < y_dim; j++){
            if(grid.get(i, j) == 1){
                image_grid[i][j] = Color(0.0);
            }

        }
    }
}

DataGrid::DataGrid(NormalGrid inGrid) {
    x_dim = inGrid.getXDim();
    y_dim = inGrid.getYDim();

    image_grid = new Color *[x_dim];
    for (int i = 0; i < x_dim; i++) {
        image_grid[i] = new Color[y_dim];
//        image_grid[i] = Color(inGrid[])
    }
    for(int i = 0; i < y_dim; i++){
        for (int j = 0; j < x_dim; j++){
            image_grid[j][i] = Color(inGrid.get(j, i).x,
                                     inGrid.get(j, i).y,
                                     inGrid.get(j, i).z);

        }
    }
}


