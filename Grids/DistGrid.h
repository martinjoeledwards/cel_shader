//
// Created by marti on 06-Nov-21.
//

#ifndef RT_DISTGRID_H
#define RT_DISTGRID_H


#include "NormalGrid.h"

class DistGrid {
public:
    DistGrid(unsigned int x_dim, unsigned int y_dim);

    DistGrid(NormalGrid grid);

    unsigned int getXDim();
    unsigned int getYDim();
    double get(unsigned int x, unsigned int y);
    double* getRef(unsigned int x, unsigned int y);
    void set(unsigned int x, unsigned int y, double input);

    void detectEdges();

    void quantize(double d);

private:
    unsigned int x_dim{};
    unsigned int y_dim{};

    double **image_grid;

    double multiply(double pDouble[9], double **pDouble1, int i, int j);
};


#endif //RT_DISTGRID_H
