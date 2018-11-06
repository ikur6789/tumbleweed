#pragma once

#include <random>
#include <iostream>

class weed
{
    public:
        weed();
        void initializeRandPosition();
        void updateVelocity(double acceleration[]);

        double position[2];
        double velocity[2];
        double fitness = 0.0;
        const double MINIMUM = -8;
        const double MAXIMUM = 8;

    private:
    	std::default_random_engine gen;
    	std::uniform_real_distribution<double> domain;
};