//
// Created by marti on 06-Sep-21.
//

#ifndef RT_HELPERS_H
#define RT_HELPERS_H
#include <string>
#include <cmath>
#include <chrono>
#include "Color.h"
#include "Point.h"

std::string get_time_string();

class Progress{
public:
    Progress(double y_dim){
        percent_complete = 0;
        std::cout << "progress = 0%" << std::endl;
        this->y_dim = y_dim;
        this->begin = std::chrono::high_resolution_clock::now();
    }
    void print_progress(unsigned int j){
        int new_perc = ((double) j / y_dim) * 100;
        if (new_perc > percent_complete){
            percent_complete = new_perc;
            std::cout << "progress = " << percent_complete << "%" << std::endl;
        }
    }
    void print_finish(){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

        std::cout << "progress = 100%" << std::endl;
        printf("\nTime elapsed: %.3f seconds.\n", elapsed.count() * 1e-9);
    }
private:
    int percent_complete;
    double y_dim;
    std::chrono::time_point<std::chrono::high_resolution_clock> begin;
};


#endif //RT_HELPERS_H
