#include "weed.h"

weed::weed()
{
	std::random_device rd;
	gen = std::mt19937(rd());
	domain = std::uniform_real_distribution<double>(MINIMUM, MAXIMUM);
}

void weed::initializeRandPosition()
{
	position[0] = domain(gen);
	position[1] = domain(gen);

	std::cout << "POSITION 1 " << position[0] << std::endl;
	std::cout << "POSITION 2 " << position[1] << std::endl;
	
}

void weed::updateVelocity(double acceleration[])
{
	velocity[0] += acceleration[0];
	velocity[1] += acceleration[1];
}

void weed::updatePosition()
{
	position[0] += velocity[0];
	position[1] += velocity[1];
}