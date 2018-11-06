#pragma once

#include <random>
#include <iostream>

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

        const double MINIMUM = -8;
        const double MAXIMUM = 8;

    private:
    	std::mt19937 gen;
    	std::uniform_real_distribution<double> domain;
};