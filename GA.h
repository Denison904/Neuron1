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
	void Save();
	void Save(string filename);
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
	int np;
	double**** dw;
	int maxStap = 0;
	int maxSource = 0;
	double*** w0;
	double*** dw0;
	int N;
	vector<double> sr;// средний шаг
};

