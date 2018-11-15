#pragma once

typedef struct {
    std::function<double(double *, unsigned int)> functionCall; // the actual evaluation function
    bool isMin;                                                // whether or not we're trying to minimize or maximize
} probFunc;

