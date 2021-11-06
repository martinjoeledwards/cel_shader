//
// Created by marti on 06-Nov-21.
//

#include "DistGrid.h"
#include <cmath>

DistGrid::DistGrid(unsigned int in_x_dim, unsigned in_y_dim) {
    x_dim = in_x_dim;
    y_dim = in_y_dim;

    image_grid = new double *[x_dim];
    for (int i = 0; i < x_dim; i++) {
        image_grid[i] = new double[y_dim];
    }
}

DistGrid::DistGrid(NormalGrid grid) {
    x_dim = grid.getXDim();
    y_dim = grid.getYDim();

    image_grid = new double *[x_dim];
    for (int i = 0; i < x_dim; i++) {
        image_grid[i] = new double[y_dim];
    }

    for(int i = 1; i < y_dim - 1; i++){
        for (int j = 1; j < x_dim - 1; j++){
            image_grid[j][i] = grid.get(j, i).x;
        }
    }
}

unsigned int DistGrid::getXDim() {
    return x_dim;
}
unsigned int DistGrid::getYDim() {
    return y_dim;
}

double DistGrid::get(unsigned int x, unsigned int y) {
    return image_grid[x][y];
}
double* DistGrid::getRef(unsigned int x, unsigned int y){
    return &image_grid[x][y];
}

void DistGrid::set(unsigned int x, unsigned int y, double input) {
    image_grid[x][y] = input;
}

void DistGrid::detectEdges() {
    double** edge_grid = new double *[x_dim];
    for (int i = 0; i < x_dim; i++) {
        edge_grid[i] = new double[y_dim];
    }

    double xkernel[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    double ykernel[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for(int i = 1; i < y_dim - 1; i++){
        for (int j = 1; j < x_dim - 1; j++){
            double x_mult = multiply(xkernel, image_grid, i, j);
            double y_mult = multiply(ykernel, image_grid, i, j);
            double total = (x_mult * x_mult) + (y_mult * y_mult);
            total = sqrt(total);
            edge_grid[j][i] = total;

//            double topleft = xkernel[0] * image_grid[j-1][i-1];

//            image_grid[j][i] = Color(inGrid.get(j, i));
//            image_grid[j][i] = Color(125);

        }
    }
    for(int i = 1; i < y_dim - 1; i++){
        for (int j = 1; j < x_dim - 1; j++){
            image_grid[j][i] = edge_grid[j][i]; //still a lot of variation in the values here
        }
    }

}

double DistGrid::multiply(double *box, double **grid, int i, int j) {

    double topleft = box[0] * grid[j-1][i-1];
    double topmid = box[1] * grid[j][i-1];
    double topright = box[2] * grid[j+1][i-1];
    double midleft = box[3] * grid[j-1][i];
    double midmid = box[4] * grid[j][i];
    double midright = box[5] * grid[j+1][i];
    double botleft = box[6] * grid[j-1][i+1];
    double botmid = box[7] * grid[j][i+1];
    double botright = box[8] * grid[j+1][i+1];
    double total = topleft + topmid + topright
            + midleft + midmid + midright
            + botleft + botmid + botright;
    return total;

}

void DistGrid::quantize(double d) {
    for(int i = 0; i < y_dim; i++){
        for (int j = 0; j < x_dim; j++){
            if(image_grid[j][i] < d) {      // no edge
                image_grid[j][i] = 0;
            } else {
                image_grid[j][i] = 255;       // edge
            }
        }
    }
}

