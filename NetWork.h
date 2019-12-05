#pragma once
#include "Neuron.h"
#include "Data.h"
#include "Game.h"
#include "FruitData.h"

class NetWork
{
public:
	double delta(double *output, double *rr);
	double act(int x);
	double Dact(int x);
	NetWork();
	NetWork(string filename);
	int max();
	double **trans(double );
	void BackPropogation( double* rr);
	void ForwardFeed();
	//void GA();
	void Save(int i, int j);
	void saveWeights();
	void Sensor(int number);
	void Start();
	//void Enter();
	void Draw(int i);
	void BookLearning( double *rr);
	int InputRR();
	double dif(double x);
	~NetWork() {};
private:
	Neuron **neuron;
	Game* game;
	Data **data;
	int Layer;
	double rr[4];
	int* size;
//	int number;
	int population;
	int radius;
	DataFruit **Fruit;
	int *stapAlive;
	double lr;
	double h;
};

