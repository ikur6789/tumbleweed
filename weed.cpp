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

}