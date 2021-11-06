//
// Created by marti on 06-Nov-21.
//

#include "NormalGrid.h"

NormalGrid::NormalGrid(unsigned int in_x_dim, unsigned in_y_dim) {
    x_dim = in_x_dim;
    y_dim = in_y_dim;

    image_grid = new Point *[x_dim];
    for (int i = 0; i < x_dim; i++) {
        image_grid[i] = new Point[y_dim];
    }


}

unsigned int NormalGrid::getXDim() {
    return x_dim;
}
unsigned int NormalGrid::getYDim() {
    return y_dim;
}

Point NormalGrid::get(unsigned int x, unsigned int y) {
    return image_grid[x][y];
}
Point* NormalGrid::getRef(unsigned int x, unsigned int y){
    return &image_grid[x][y];
}

void NormalGrid::set(unsigned int x, unsigned int y, Point input) {
    image_grid[x][y] = input;
}

void NormalGrid::colorize() {
    for(int i = 0; i < x_dim; i++){
        for (int j = 0; j < y_dim; j++){
//            std::cout << "\ninitially ";
//            image_grid[i][j].Print();
            double x = image_grid[i][j].x;
            double y = image_grid[i][j].y;
            double z = image_grid[i][j].z;

            std::cout << "x = " << x;
            std::cout << "\ny = " << y;
            std::cout << "\nz = " << z << "\n";

            Point here = Point(x, y, z);
            std::cout << "before mult:";
            image_grid[i][j].Print();
            Point multiplied = image_grid[i][j] * 128;
            std::cout << "after mult:";
            multiplied.Print();
            multiplied = multiplied + 128;
            if(multiplied.x < 0
            || multiplied.y < 0
            || multiplied.z < 0){
                std::cout << "failed ";
                multiplied.Print();
                exit(1);
            }

            image_grid[i][j] = multiplied;

//            image_grid[i][j] = here;
//            std::cout << "after mult ";
//            here.Print();
//            image_grid[i][j].Print();
//            image_grid[i][j] = (image_grid[i][j] * 128) + 128;


//            image_grid[i][j] = absVal(image_grid[i][j] * 300);
//            std::cout << "and after, ";
//            image_grid[i][j].Print();
            //            ((image_grid[i][j] * 128) +128).Print();

        }
    }
}
