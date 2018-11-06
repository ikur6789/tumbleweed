#pragma once

#include <iostream>
#include <weedRand.hpp>

class weed
{
    public:
        weed();
        void initializeRandPosition();
        void updateVelocity(double acceleration[]);
		void updatePosition();

        double position[2];
        double velocity[2];
        double fitness = 0.0;

};