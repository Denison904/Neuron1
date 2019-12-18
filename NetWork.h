#pragma once
#include "Neuron.h"
#include "Data.h"
#include "Game.h"
#include "FruitData.h"


class NetWork
{
public:
	double delta(double *output, double *rr);
	double act(double x);
	double Dact(double x);
	NetWork();
	NetWork(string filename);
	int max();
	double **trans(double );
	void BackPropogation( double rr);
	void ForwardFeed();
	//void GA();
	void Save(int i, int j);
	void saveWeights();
	void Sensor(int number);
	void Start();
	//void Enter();
	void Draw(int i);
	void BookLearning( double *rr, double *predict);
	int InputRR(int  i);
	double dif(double x);
	double norma(double*** dw, double ***w);
	~NetWork() {};
private:
	Game *game;
	double** value;
	double*** weights;
	double** w0;
	int Layer;
	int* size;
	int radius;
	double lr;
	double h;
	double*** dw;
	double** dw0;
	double **save;
	double* rr;

};