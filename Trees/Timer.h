#pragma once

#include <ctime>

class Timer {
public:

    Timer();
    clock_t update();

private:

    clock_t last;

};
