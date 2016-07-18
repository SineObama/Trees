#include "stdafx.h"
#include "Timer.h"

Timer::Timer() {
    last = 0;
}

clock_t Timer::update() {
    clock_t cur = clock(), rtn = cur - last;
    last = cur;
    return rtn;
}
