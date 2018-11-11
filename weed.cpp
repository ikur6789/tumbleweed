#include "weed.h"

weed::weed()
{

}

void weed::initializeRandPosition()
{
	position[0] = weedRand::getInstance().random();
	position[1] = weedRand::getInstance().random();

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

void weed::drag()
{
	if(velocity[0] < 0)
		velocity[0] += 0.001;
	if(velocity[1] < 0)
		velocity[1] += 0.001;
	if(velocity[0] > 0)
		velocity[0] -= 0.001;
	if(velocity[1] > 0)
		velocity[1] -= 0.001;
}