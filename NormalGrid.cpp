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
