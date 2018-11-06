#include "weed.h"

weed::weed()
{
	domain = std::uniform_real_distribution<double>(MINIMUM, MAXIMUM);
}

void weed::initializeRandPosition()
{
	position[0] = domain(gen);
	position[1] = domain(gen);
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