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

void weed::setVelocity(double velocity[])
{
	this->velocity[0] = velocity[0];
	this->velocity[1] = velocity[1];
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
		{ 
			if (velocity[i] < 0) 
				velocity[i] += DRAG;
			else
				velocity[i] -= DRAG;
		}
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

void weed::windOperations()
{
	//Placeholder for now to determine if wind functions should be handled by weeds, but probably should not.
	//Wind may be able to be it own class
}

/* min and max - step size */
void weed::doRandomSearch(double min, double max, probFunc *pf)
{
	/* Get the current position */
	double testPos[2] = {position[0], position[1]};

	/* Get a random direction */
	double dir0 = min + (double)rand() / ((double)RAND_MAX / (max - min));
	double dir1 = min + (double)rand() / ((double)RAND_MAX / (max - min));

	//std::cout << "min,max " << min << " " << max << std::endl;

	/* Add the random direction to the solution */
	testPos[0] += dir0;
	testPos[1] += dir1;

	//std::cout << "Test Dir 0, 1: " << dir0 << " " << dir1 << std::endl;

	/* See if that new solution is better; if so, update
	 * velocity with said direction */\
	double tempFitness = pf->functionCall(testPos, 2);
	//std::cout << "Temp fitness, curfitness: " << tempFitness << " " << fitness << std::endl;
	if (tempFitness > fitness) {
		//std::cout << "TAKING TEMP FITNESS!\n";
		position[0] = testPos[0];
		position[1] = testPos[1];
		fitness = tempFitness;
	}
}

