#include "Snake.h"

Snake::Snake() {
	x = high / 2;
	y = width / 2;
	body = 4;
	Tail = new vector<int>[2];
	for (int i = 0; i < body; i++)
	{
		//int tmp[2] = { x + i + 1,y };
		Tail[0].push_back(x+i+1);
		Tail[1].push_back(y);
	}
	course = 0;
	Alive = true;
	
}

void Snake::setCourse(int course) {
	this->course = course;
}

bool Snake::getAlive() {
	return Alive;
}

int Snake::getX() {
	return x;
}
int Snake::getY() {
	return y;
}

int Snake::getBody() {
	return body;
}

vector<int>* Snake::getTail() {
	return Tail;
}

void Snake::setBody() {
	body++;
}
void Snake::setAlive() {
	Alive = false;
}

void Snake::MoveTail() {


	for (int i = getBody() - 1; i > 0; i--)
	{
		Tail[0][i] = Tail[0][i - 1];
		Tail[1][i] = Tail[1][i - 1];
	}
	switch (course)
	{
	case 0:
		Tail[0][0] = x + 1;
		Tail[1][0] = y;
		break;
	case 1:
		Tail[0][0] = x;
		Tail[1][0] = y + 1;
		break;
	case 2:
		Tail[0][0] = x - 1;
		Tail[1][0] = y;
		break;
	case 3:
		Tail[0][0] = x;
		Tail[1][0] = y - 1;
		break;
	}

}

void Snake::setTail(int x,int y) {
	int tmp[2];
	tmp[0] = Tail[0].back();
	tmp[1] = Tail[1].back();
	MoveTail();		
	Tail[0].push_back(tmp[0]);
	Tail[1].push_back(tmp[1]);
}


void Snake::setX(int x) {
	this->x += x;
}

void Snake::setY(int y) {
	this->y += y;
}

int Snake::getCourse() {
	return course;
}
