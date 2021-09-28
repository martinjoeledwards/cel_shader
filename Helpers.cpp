//
// Created by marti on 06-Sep-21.
//
#if defined (_MSC_VER)  // Visual studio
#define thread_local __declspec( thread )
#elif defined (__GCC__) // GCC
#define thread_local __thread
#endif

#include <random>
#include <thread>
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

int intRand(const int & min, const int & max) {


    static thread_local std::mt19937* generator = nullptr;
    auto test = std::hash<std::thread::id>()(std::this_thread::get_id());
    if (!generator) generator = new std::mt19937(clock() + test);
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(*generator);
}


double getRandDouble(double min_val, double max_val) {

    double normalized = double(intRand(0, 99999)) / (double(99999) + 1.0);
    return ((max_val - min_val) * normalized) + min_val;
}
