//
// Created by marti on 06-Nov-21.
//

#ifndef RT_NORMALGRID_H
#define RT_NORMALGRID_H


#include "Color.h"

class NormalGrid {
public:
    NormalGrid(unsigned int x_dim, unsigned int y_dim);
    unsigned int getXDim();
    unsigned int getYDim();
    Point get(unsigned int x, unsigned int y);
    Point* getRef(unsigned int x, unsigned int y);
    void set(unsigned int x, unsigned int y, Point input);

private:
    unsigned int x_dim{};
    unsigned int y_dim{};

    Point **image_grid;

};


#endif //RT_NORMALGRID_H
