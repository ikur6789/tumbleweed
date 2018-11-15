#pragma once

#include <random>

class weedRand
{
private:

    	std::mt19937 gen;

    	std::uniform_real_distribution<double> domain;

	const double MINIMUM = -8.0;

	const double MAXIMUM = 8.0;

private:

	weedRand()
	{
		std::random_device rd;

		gen = std::mt19937(rd());

		domain = std::uniform_real_distribution<double>(MINIMUM, MAXIMUM);
	}

public:

	static weedRand& getInstance()
	{
		static weedRand instance;

		return instance;
	}

	double random()
	{
		return domain(gen);
	}
};
