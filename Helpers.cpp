//
// Created by marti on 06-Sep-21.
//

#include "Helpers.h"

std::string get_time_string(){
    time_t curr_time = time(0);
    tm *ltm = localtime(&curr_time);

    auto month = std::to_string(1 + ltm->tm_mon);
    auto day = std::to_string(ltm->tm_mday);
    auto hour = std::to_string(ltm->tm_hour);
    auto min = std::to_string(ltm->tm_min);
    auto sec = std::to_string(ltm->tm_sec);

    return month + "_" + day + "_" + hour + "-" + min + "-" + sec;
}

double getRandDouble(double min_val, double max_val) {
    double normalized = double(rand()) / (double(RAND_MAX) + 1.0);
    return ((max_val - min_val) * normalized) + min_val;
}
