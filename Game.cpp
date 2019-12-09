#include "Game.h"

Game::Game()
{
	stap = 0;
	source = 0;
	stop = 50;

	fruit = new int* [10];
	//snake = Snake();

	for (int i = 0; i < 10; i++)
	{
		fruit[i] = new int[2];
		fruit[i][0] = rand() % (width - 2) + 1;
		fruit[i][1] = rand() % (high - 2) + 1;
	}
}
/*
Game::~Game()
{	
	for (int i = 0; i < 10; i++)
	{
		delete[] fruit[i];
	}
	delete[] fruit;
//	delete snake;
}*/

void Game::Input(int input) {
	snake.setCourse(input);
}



void Game::Draw() {
	if (snake.getAlive())
	{
		//system("cls");
		for (int i = 0; i < high; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (i == 0 || j == 0 || i == high - 1 || j == width - 1)
				{
					cout << "#";
				}
				else if (i == snake.getY() && j == snake.getX())
				{
					cout << "@";
				}
				else if (CheckTail(j, i))
				{
					cout << "o";
				}
				else  if (CheckFruit(j, i)) {
					cout << "F";
				}
				else cout << " ";
			}
			cout << '\n';
		}
		cout << "Stap: " << stap << endl;
	}
	
}

bool Game::CheckTail(int x, int y) {
	vector<int> *tmp;
	tmp = snake.getTail();
	for (int i = 0; i < snake.getBody(); i++)
	{
		if (tmp[0][i] == x && tmp[1][i] == y)
			return true;
	}
	return false;
}

bool Game::CheckFruit(int x, int y) {
	for (int i = 0; i < 10; i++)
	{
		if (fruit[i][0] == x && fruit[i][1] == y)
			return true;
	}
	return false;

}




void Game::Logic() {
	switch (snake.getCourse())
	{
	case 0: snake.setX(-1); break;
	case 1: snake.setY(-1); break;
	case 2: snake.setX(1); break;
	case 3: snake.setY(1); break;
	}
	bool eat = false;
	
	for (int i = 0; i < 10; i++)
	{
		if (snake.getX() == fruit[i][0] && snake.getY() == fruit[i][1]) {
			vector<int> *tmpTail;
			tmpTail = snake.getTail();
			snake.setTail(tmpTail[0].back(),tmpTail[0].back());
			snake.setBody();
			
			while (CheckTail(fruit[i][0], fruit[i][1]) || (fruit[i][0] == snake.getX() && fruit[i][1] == snake.getY()))
			{
				fruit[i][0] = rand() % (width - 2) + 1;
				fruit[i][1] = rand() % (high - 1) + 1;
			}
			eat = true;
		}

	}
	if (!eat) {
		snake.MoveTail();
	}
	

	if (snake.getX() <= 0 || snake.getX() >= width - 1 || snake.getY() <= 0 || snake.getY() >= high - 1) {
		snake.setAlive();
	//	system("cls");
		//cout << "GAME OVER 1!\nSOURSE = " << getSource() << "\nIteration = " << stap << endl;


	}
	else if (CheckTail(snake.getX(), snake.getY()))
	{
		snake.setAlive();
		//system("cls");
		//cout << "GAME OVER! 2\nSOURCE = " << getSource() << "\nIteration = " << stap << endl;
	}

	if (stap >= stop)
	{
		snake.setAlive();
	}
	stap++;
}

int Game::getSource() {
	return source;
}


int Game::getStap() {
	return  stap;
}

int** Game::getFruit() {
	return fruit;
}

int* Game::getCoord() {
	int* tmp;
	tmp = new int[2];
	tmp[0] = snake.getX();
	tmp[1] = snake.getY();
	return tmp;
}

vector<int>* Game::getTail() {
	return snake.getTail();
}
 
bool Game::CheckBorder(int x, int y) {
	if (x <= 0 || x >= width - 1 || y <= 0 || y >= high - 1) {
		return true;
	}
	else
	{
		return false;
	}
}
/*
bool Game::CheckFruit(int x, int y) {
	for (int i = 0; i < 10; i++)
	{
		if (x == fruit[i][0]&&y ==fruit[i][1])
		{
			return true;
		}
	}
	return false;
}*/

Game::Game(int **fruit) {
	stap = 0;
	source = 0;
	stop = 50;


	this->fruit = fruit;
}

bool Game::getAlive() {
	return snake.getAlive();
}

void Game::setAlive() {
	//snake.setAlive();
	snake = Snake();
	for (int i = 0; i < 10; i++)
	{
		fruit[i][0] = rand() % (width - 2) + 1;
		fruit[i][1] = rand() % (high - 2) + 1;
		if (CheckTail(fruit[i][0], fruit[i][1])) {
			i--;
		}
	}
	stap = 0;
	source = 0;
	stop = 50;
}