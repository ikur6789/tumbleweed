#include "weed.h"

weed::weed()
{

}

void weed::initializeRandPosition()
{
	position[0] = weedRand::getInstance().random();
	position[1] = weedRand::getInstance().random();

	//std::cout << "POSITION 1 " << position[0] << std::endl;
	//std::cout << "POSITION 2 " << position[1] << std::endl;
}

void weed::updateVelocity(double acceleration[])
{
	velocity[0] += acceleration[0];
	velocity[1] += acceleration[1];
}

void weed::applyDrag()
{
	for (int i = 0; i < 2; i++) 
	{
		/* If it's close enough to 0 just stop */
		if (abs(velocity[i]) < DRAG) 
			velocity[i] = 0;

		else 
			if (velocity[i] < 0) 
				velocity[i] += DRAG;
			else
				velocity[i] -= DRAG;
	}
}

void weed::updatePosition()
{
	position[0] += velocity[0];
	position[1] += velocity[1];

	for (int i = 0; i < 2; i++) 
	{
		/* If we hit a wall start in a new random position */
		if (position[i] < MINIMUM || position[i] > MAXIMUM) 
		{
			initializeRandPosition();
			break;
		}
	}
}
