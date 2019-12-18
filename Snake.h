#pragma once
#include "Global.h"
//#include <array>



class Snake
{
public:
	Snake();
	Snake(int body);
	void setCourse(int course);
	bool getAlive();
	int getX();
	int getY();
	int getBody();
	void setBody();
	void setAlive();
	void MoveTail();
	void setTail(int x, int y);
	void setX(int x);
	void setY(int y);
	int getCourse();
	vector<int>* getTail();
private:
	int x, y;
	//int Tailcoord[2];
	vector<int> Tail[2];
	int course;
	//int *vision;
	bool Alive;
	int body;

};

