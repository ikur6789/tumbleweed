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

#define DRAG  0.0001

class weed
{
    public:
        weed();
        void initializeRandPosition();
        void updateVelocity(double acceleration[]);
        void applyDrag();
		void updatePosition();

        void doRandomSearch(double, double, probFunc *);

        double position[2];
        double velocity[2];
        double fitness = 0.0;

};