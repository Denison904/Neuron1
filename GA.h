#pragma once
#include "Global.h"
#include "Data.h"
#include "Game.h"
//#include "Neuron.h"




class GA
{
public:
	GA();
	double act(double x);
	//template<typename T1, typename T2>
	void crooss(int a, int b );
	void Mutation();
	void ForwardFeed(int pop);
	void SortSource();
	void SortStep();
	void Start();
	void Sensor(int i);
	void Draw(int i);
	int max();
	void StartDraw(int i);
	void StartNoDraw(int i);
private:
	int generation;
	Game *game;
	int	radius;
	int Layer;
	int* sizeLayer;
	double** value;
	double**** weight;
	int population;
	double*** tmpv;
	Game tmp;
	//vector<int> stapAlive;
	int np;
};

