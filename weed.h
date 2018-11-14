#pragma once

#include <iostream>
#include <weedRand.hpp>

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
        void windOperations(); //See .cpp file

        double position[2];
        double velocity[2];
        double fitness = -999.0;

};