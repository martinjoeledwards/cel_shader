//
// Created by marti on 06-Sep-21.
//

#ifndef RT_DATAGRID_H
#define RT_DATAGRID_H
#include "../Color.h"
#include "DistGrid.h"
#include "NormalGrid.h"

class DataGrid {
public:
    DataGrid(unsigned int x_dim, unsigned int y_dim);
    explicit DataGrid(DistGrid);
    explicit DataGrid(NormalGrid inGrid);

    unsigned int getXDim();
    unsigned int getYDim();
    Color get(unsigned int x, unsigned int y);
    Color* getRef(unsigned int x, unsigned int y);
    void set(unsigned int x, unsigned int y, Color input);
    void save_image(std::string folder = "renders");


    void addEdges(DistGrid grid);

private:
    unsigned int x_dim{};
    unsigned int y_dim{};

    Color **image_grid;


};




#endif //RT_DATAGRID_H
