#include "GA.h"
#include <thread>
#include <chrono>

GA::GA() {
	cout << "Enter number of poppulation\n";
	cin >> population;
	game = new Game[population];
	cout << "Enter number of radius\n";
	cin >> radius;
	cout << "enter number of layer\n";
	cin >> Layer;
	sizeLayer = new int[Layer];
	sizeLayer[0] =int(pow(radius * 2 + 1, 2))-1;


	//vector<double> t(int(pow(radius*2+1,2))-1);
	value = new double* [Layer];
	value[0] = new double[sizeLayer[0]];
	for (int i = 1; i < Layer-1; i++)
	{
		cout << "Enter number of layers\n";
		sizeLayer[i] = _getch();
		value[i] = new double[sizeLayer[i]];
	}
	sizeLayer[Layer-1] = 4;
	value[Layer - 1] = new double[sizeLayer[Layer - 1]];
	weight = new double*** [population];
	for (int p = 0; p < population; p++)
	{
		weight[p] = new double** [Layer];
		for (int i =1; i < Layer; i++)
		{
			weight[p][i] = new double* [sizeLayer[i]];
			for (int j = 0; j < sizeLayer[i]; j++)
			{
				weight[p][i][j] = new double[sizeLayer[i - 1]];
				for (int k = 0; k < sizeLayer[i-1]; k++)
				{
					srand(time(NULL));
					weight[p][i][j][k] = ((rand() % 200 - 100) * 0.01);
				}
			}
		}
	}

	generation = 1;
	np = population / 2;
	Start();
}

//template<typename T1, typename T2>
void GA::crooss(int a, int b) {
	for (int i = 1; i < Layer; i++)
	{
		for (int j = 0; j < sizeLayer[i]; j++)
		{
			int r;
			if (a<sqrt(population)/2)
			{
				srand(time(NULL));
				r = rand() % int(sizeLayer[i-1]/2+0.5)+int(sizeLayer[i-1]/2+0.5);
			}
			else
			{
				srand(time(NULL));
				r = rand() % (sizeLayer[i - 1] - 2);

			}
			
			for (int k = 0; k < sizeLayer[i-1]; k++)
			{
				if (weight[a][i][j][k] != weight[b][i][j][k] ) {
					if (k<= r) {
						weight[a][i][j][k] = weight[b][i][j][k];
					}
					else if (k > r)
					{
						weight[b][i][j][k] = weight[a][i][j][k];
					}

				}
			}
		}
	}

}

void GA::SortStep() {
	for (int i = 0; i < population; i++)
	{
		for (int j = 0;  j < population-1; j++)
		{
			if ( game[j].getStap()< game[j+1].getStap()) {
				
				tmp	= game[j+1];
				game[j] = game[j+1];
				game[j] = tmp;
				
				tmpv = weight[j];
				weight[j] = weight[j+1];
				weight[j+1] = tmpv;

			}
		}
		
	}

}

void GA::SortSource() {
	int a = int(sqrt(population));
	
	Game tmp;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j <a - 1; j++)
		{
			if (game[j].getSource() < game[j + 1].getSource()) {
				
				tmp = game[j + 1];
				game[j] = game[j + 1];
				game[j] = tmp;

				tmpv = weight[j];
				weight[j] = weight[j + 1];
				weight[j + 1] = tmpv;
			}
		}

	}
}

void GA::Start() {

	int thr = thread::hardware_concurrency();
	thread* th;
	th = new thread[thr];
	int newpop = population / thr;
	do
	{
		/*
		for (int i = 0; i < population; i++)
		{
			while (game[i].getAlive())
			{


				Sensor(i);
				ForwardFeed(i);
				Draw(i);
				game[i].Input(max());
				game[i].Logic();

				//j++;
			}

		}
		*/
		for (int i = 0; i < newpop; i++)
		{
			th[0] = thread(&GA::StartDraw,this, 4*i);
			for (int j = 1; j < thr; j++)
			{
				th[j] = thread(&GA::StartNoDraw,this, 4 * i+j);
			}
			for (int j = 0; j < thr; j++)
			{
				th[j].join();
			}
			

		}
		SortStep();
		SortSource();
		fstream f;
		f.open("GA1.txt", ios_base::trunc);
		f << Layer << "\n";
		for (int i = 1; i < Layer; i++)
		{
			for (int j = 0; j < sizeLayer[i]; j++)
			{
				for (int k = 0; k < sizeLayer[i-1]; k++)
				{
					f << weight[0][i][j][k] << " ";
				}
				f << "\n";
			}
		}
		f.close();
		for (int i = 0; i < int(sqrt(population)); i++)
		{
			for (int j = 0; j < int(sqrt(population)); j++)
			{
				crooss(i,j);
			}
		}
		Mutation();
		//delete game;
		
		for (int i = 0; i < population; i++)
		{
			game[i].setAlive();
			
			//[i] = Game();
		}
		generation++;
	} while (true);
}

void GA::ForwardFeed(int pop) {
	for (int i = 1; i < Layer; i++)
	{
		for (int j = 0; j < sizeLayer[i]; j++)
		{
			double arg = 0;
			for (int k = 0; k < sizeLayer[i-1]; k++)
			{
				arg += value[i - 1][k] * weight[pop][i][j][k];
			}
			value[i][j] = act(arg);

		}
	}
}


double GA::act(double x) {
	return 2.0 / (1 + exp(-x)) - 1;
}


void GA::Sensor(int number) {
	int* coor;
	coor = game[number].getCoord();

	coor[0] -= radius;
	coor[1] -= radius;
	int k = 0;
	for (int i = 0; i < 2 * radius + 1; i++)
	{
		for (int j = 0; j < 2 * radius + 1; j++)
		{
			if ((i != radius || j != radius)) {
				if (game[number].CheckBorder(coor[0] + j, coor[1] + i))
				{
					value[0][k++] = -1;
				}
				else if (game[number].CheckTail(coor[0] + j, coor[1] + i))\
				{
					value[0][k++] = -1;
				}
				else if (game[number].CheckFruit(coor[0] + j, coor[1] + i))
				{
					value[0][k++]= 1;
				}
				else
				{
					value[0][k++]= 0;
				}
			}
		}
	}
}

int GA::max() {
	int index = 0;
	int max = value[Layer - 1][0];
	for (int i = 1; i < sizeLayer[Layer - 1]; i++)
	{
		if (value[Layer - 1][i]> max) {
			index = i;
			max = value[Layer - 1][i];
		}
	}
	return index;
}



void GA::Draw(int n) {
	if (!game[n].getAlive())
		return;
	int k = 0;
	system("cls");
	game[n].Draw();
	cout << "Generation: " << generation << endl;
	cout << "Snake: " << n << endl;
	for (int i = 0; i < sizeLayer[Layer-1]; i++)
	{
		cout << value[Layer - 1][i] << '\t';
	}
	cout << endl;
	
	for (int i = 0; i < sizeLayer[0]; i++)
	{
		cout << value[0][i] << '\t';
	}
}

void GA::StartDraw(int i) {
	while (game[i].getAlive())
	{


		Sensor(i);
		ForwardFeed(i);
		Draw(i);
		game[i].Input(max());
		game[i].Logic();

		//j++;
	}
}

void GA::StartNoDraw(int i) {
	while (game[i].getAlive())
	{


		Sensor(i);
		ForwardFeed(i);
	//	Draw(i);
		game[i].Input(max());
		game[i].Logic();

		//j++;
	}

}

void GA::Mutation() {
	for (int i = np; i <population; i++)
	{
		for (int j = 1; j < Layer; j++)
		{
			for (int k = 0; k < sizeLayer[j]; k++)
			{
				srand(time(NULL));
				int p;
				p = rand() % sizeLayer[j - 1];
				weight[i][j][k][p] = rand() % 200 * 0.01 - 1;

			}
		}
	}
}