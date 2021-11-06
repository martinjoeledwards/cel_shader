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

            Point multiplied = image_grid[i][j] * 128;
            multiplied = multiplied + 128;
            image_grid[i][j] = multiplied;

        }
    }
}

void NormalGrid::quantize(double d) {
    for(int i = 0; i < y_dim; i++){
        for (int j = 0; j < x_dim; j++){
            if(image_grid[j][i].x > d
            || image_grid[j][i].y > d
            || image_grid[j][i].z > d ) {
                image_grid[j][i] = Point(255, 255, 255);
            } else {
                image_grid[j][i] = Point(0, 0, 0);       // edge
            }
        }
    }
}

void NormalGrid::detectEdges() {

    auto edge_grid = new Point *[x_dim];
    for (int i = 0; i < x_dim; i++) {
        edge_grid[i] = new Point [y_dim];
    }

    double xkernel[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    double ykernel[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for(int i = 1; i < y_dim - 1; i++){
        for (int j = 1; j < x_dim - 1; j++){
            Point x_mult = multiply(xkernel, image_grid, i, j);
            Point y_mult = multiply(ykernel, image_grid, i, j);
//            std::cout << "mults are\n";
//            x_mult.Print();
//            y_mult.Print();
            Point xsq(x_mult.square());
            Point ysq(y_mult.square());
//            xsq.Print();
//            ysq.Print();
            Point total(xsq + ysq);
//            total.Print();
            total.root();
//            total.Print();

//            Point myP(.5, .5, .5);
//            std::cout << "myP is ";
//            myP.Print();
//            myP.square();
//            myP.Print();

//            total.Print();
//            total = total.root();
////            total.Print();
//
            edge_grid[j][i] = total * 10;
//            edge_grid[j][i].Print();

        }
    }
    for(int i = 1; i < y_dim - 1; i++){
        for (int j = 1; j < x_dim - 1; j++){
            image_grid[j][i] = edge_grid[j][i]; //still a lot of variation in the values here
//            image_grid[j][i].Print();
        }
    }
}

Point NormalGrid::multiply(double *box, Point **grid, int i, int j) {

    Point topleft = grid[j-1][i-1] * box[0];
    Point topmid = grid[j][i-1] * box[1];
    Point topright = grid[j+1][i-1] * box[2];
    Point midleft = grid[j-1][i] * box[3];
    Point midmid = grid[j][i] * box[4];
    Point midright = grid[j+1][i] * box[5];
    Point botleft = grid[j-1][i+1] * box[6];
    Point botmid = grid[j][i+1] * box[7];
    Point botright = grid[j+1][i+1] * box[8];
    Point total = topleft + topmid + topright
                   + midleft + midmid + midright
                   + botleft + botmid + botright;
    return total;
}