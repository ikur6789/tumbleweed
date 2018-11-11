#pragma once

#include <random>

class weedRand
{
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

	private:
		weedRand()
		{
			std::random_device rd;
			gen = std::mt19937(rd());
			domain = std::uniform_real_distribution<double>(MINIMUM, MAXIMUM);
			
		};

    	std::mt19937 gen;
    	std::uniform_real_distribution<double> domain;
		
		double MINIMUM = -8;
		double MAXIMUM = 8;
};