#pragma once
#include "Snake.h"
#include "Global.h"

class Game
{
public:
	void Logic();
	void Input(int input);
	void Draw();
	//Game(int radius);
	Game();
	Game(int** fruit);
//	~Game();
	int getSource();
	bool CheckTail(int x, int y);
	bool CheckFruit(int x,int y);
	int getStap();
	int** getFruit();
	int* getCoord();
	vector<int>* getTail();
//	bool CheckFruit(int x, int y);
	bool CheckBorder(int x, int y);
	bool getAlive();
	void setAlive();
private:
	Snake snake;
	int **fruit;
	int stop;
	int source;
	int stap;
};

