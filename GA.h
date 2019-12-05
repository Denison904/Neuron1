#pragma once
#include "Global.h"
#include "Data.h"
#include "Game.h"
#include "Neuron.h"
class GA
{
public:
	GA();
	void crooss();
	void mutation();
	void sort();
	void setGeneration();
	void FrowardFeed();
	void SortSource();
	void SortStep();

private:
	int generation;
	Game* game;
	int	radius;
	int Layer;
	int* size;
	Neuron*** neuron;
	int* input;
	int population;
};

