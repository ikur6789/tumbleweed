#pragma once

#include <vector>
#include <iostream>
#include <weedRand.hpp>
#include <functional>

#include "probFunc.hpp"

#ifndef MINIMUM
#define MINIMUM -8
#define MAXIMUM 8
#endif

//#define DRAG  0.0001

class weed
{
    public:
        weed();
        void initializeRandPosition();
        void updateVelocity(double acceleration[]);
        void setVelocity(double velocity[]);
        void applyDrag();
		void updatePosition();
        void windOperations(); //See .cpp file

        void doRandomSearch(double, double, probFunc *);

        double position[2];
        double velocity[2];
        double fitness = -999.0; // in jacob's branch this was 0.0

        unsigned char r = 255, g = 255, b = 255;

        static double DRAG;
};
