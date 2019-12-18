#include "GA.h"
#include <thread>
#include <chrono>



GA::GA() {
	cout << "Enter number of poppulation\n";
	std::cin >> population;
	game = new Game[population];
	cout << "Enter number of radius\n";
	std::cin >> radius;
	cout << "enter number of layer\n";
	std::cin >> Layer;
	sizeLayer = new int[Layer];
	sizeLayer[0] =int(pow(radius * 2 + 1, 2))-1;


	//vector<double> t(int(pow(radius*2+1,2))-1);
	value = new double* [Layer];
	value[0] = new double[sizeLayer[0]];
	for (int i = 1; i < Layer-1; i++)
	{
		std::cout << "Enter size of layers\n";
		std::cin>>sizeLayer[i] ;
		value[i] = new double[sizeLayer[i]];
	}
	sizeLayer[Layer - 1] = 1;// 4;
	value[Layer - 1] = new double[sizeLayer[Layer - 1]];
	weight = new double*** [population];
	w0 = new double** [population];
	
	srand(time(NULL));
	for (int p = 0; p < population; p++)
	{
		weight[p] = new double** [Layer];
		w0[p] = new double*[Layer];
		//srand(time(NULL));
		 
		for (int i = 1; i < Layer; i++)
		{
			w0[p][i] = new double[sizeLayer[i]];
			weight[p][i] = new double* [sizeLayer[i]];
			for (int j = 0; j < sizeLayer[i]; j++)
			{
				w0[p][i][j] = rand() % 200 * 0.01 - 1;
				weight[p][i][j] = new double[sizeLayer[i - 1]];
				for (int k = 0; k < sizeLayer[i - 1]; k++)
				{

					weight[p][i][j][k] = rand() % 200*0.01 - 1;
				}
			}
		}
	}
	dw0 = w0;
	dw = weight;
	generation = 0;
	np = population / 2;
	cin >> N;

	Start();

}

//template<typename T1, typename T2>
void GA::crooss(int a, int b) {
	srand(time(NULL));
	for (int i = 1; i < Layer; i++)
	{
		int r = 0;
		for (int j = 0; j < sizeLayer[i]; j++)
		{

			if (a < sqrt(population) / 2)
			{

				r = rand() % int(sizeLayer[i - 1] / 2 + 0.5) + int(sizeLayer[i - 1] / 2 + 0.5);
			}
			else
			{

				r = rand() % (sizeLayer[i - 1] - 2);

			}

			for (int k = 0; k < sizeLayer[i - 1]; k++)
			{
				if (k <= r) {
					dw[a][i][j][k] = weight[a][i][j][k];
				}
				else 
				{
					dw[a][i][j][k] = weight[b][i][j][k];
				}
					
			}
			r = rand() % 1;
			if (r > j)
				dw0[a][i][j] = w0[a][i][j];
			else
				dw0[a][i][j] = w0[b][i][j];
			}


	}

}




void GA::SortStep() {
	for (int i = 0; i < population; i++)
	{
		for (int j = 0;  j < population-1; j++)
		{
			if ( game[j].getStap()< game[j+1].getStap()) {
				

				tmp = game[j];
				game[j] = game[j + 1];
				game[j + 1] = tmp;

				tmpv = weight[j];
				weight[j] = weight[j + 1];
				weight[j + 1] = tmpv;
			}
		}
		
	}
//	if (maxStap < game[0].getStap())
		maxStap = game[0].getStap();
		maxSource = game[0].getSource();
}

void GA::SortSource() {
	int a = int(sqrt(population));
	
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j <a - 1; j++)
		{
			if (game[j].getSource() < game[j + 1].getSource()) {
				
				tmp = game[j];
				game[j] = game[j+1];
				game[j+1] = tmp;

				tmpv = weight[j];
				weight[j] = weight[j + 1];
				weight[j +1] = tmpv;
			}
		}

	}
	maxStap = game[0].getStap();

	if (maxSource < game[0].getSource())
		maxSource = game[0].getSource();
	
}

void GA::Start() {
/*
	int thr = thread::hardware_concurrency();
	thread* th;
	th = new thread[thr];
	int newpop = population / thr;*/
	int** tmpFruit;
	tmpFruit = new int* [1];
	tmpFruit[0] = new int[2];

	bool stop = false;
	do
	{


		for (int i = 0; i < population; i++)
		{
			while (game[i].getAlive())
			{
				Sensor(i);
				ForwardFeed(i);
				Draw(i);
				game[i].Input(max());
				game[i].Logic();
			}
			if (i == 0)
				sr.push_back(game[i].getStap());
			else
				sr[generation] += game[i].getStap();

		}
		sr[generation] /= population;
		/*

		for (int p = 0; p < newpop; p++)
		{
			th[0] = thread(&GA::StartDraw,this, 4*p);
			for (int j = 1; j < thr; j++)
			{
				th[j] = thread(&GA::StartNoDraw,this, 4 * p+j);
			}
			for (int j = 0; j < thr; j++)
			{
				th[j].join();
			}
		}
		*/
		SortStep();
		//	SortSource();


		Save();
		//int cur = 0;
		dw = weight;
		dw0 = w0;
		for (int i = 0; i < int(sqrt(population)); i++)
		{
			for (int j = 0; j < int(sqrt(population)); j++)
			{
				if (i != j)
				{
					crooss(i, j);
				}
				//else cur++;
			}
		}
		weight = dw;
		w0 = dw0;
		Mutation();
		//delete game;


		generation++;
		if (generation%N==0) {
			double answerTest[4][9];
			tmpFruit[0][0] = width / 2 - 1*rand()%1;
			tmpFruit[0][1] = high / 2 - pow(-1,rand()%1);
			for (int i = 0; i < 4; i++)
			{

				game[i] = Game(tmpFruit, 1);

				int stap = 0;
				while (game[i].getAlive())
				{
					Sensor(i);
					ForwardFeed(i);
					Draw(i);
					game[i].Input(max());
					game[i].Logic();
					answerTest[i][stap] = value[Layer - 1][0];
					stap++;
					if (stap >= 8)
						break;
				}
			}
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					cout << answerTest[i][j] << "\t";
				}
				cout << endl;
			}
			cout << "\nDo u want stop ?\n";
			char a;
			std::cin >> a;
			if (a == 's')
				stop = true;
		}	
		for (int i = 0; i < population; i++)
		{
			game[i].setAlive();
		}
		
	} while (!stop);



	std::cin >> tmpFruit[0][0];
	sr.clear();
}

void GA::ForwardFeed(int pop) {
	for (int i = 1; i < Layer; i++)
	{
		for (int j = 0; j < sizeLayer[i]; j++)
		{
			double arg = w0[pop][i][j];
			for (int k = 0; k < sizeLayer[i-1]; k++)
			{
				double a1 = weight[pop][i][j][k];
				arg += value[i - 1][k] * weight[pop][i][j][k];
			}
			value[i][j] = act(arg);

		}
	}
}


double GA::act(double x) {
	return 2.0 / (1 + exp(-x)) - 1;
//	if (x <= 0) return 0;
//	if (x > 0) return x;
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
	delete[] coor;
}

int GA::max() {
	if (value[Layer - 1][0]>= -1.0 && value[Layer - 1][0]<= -0.5) return 0;
	if (value[Layer - 1][0] > -0.5 && value[Layer - 1][0]<= 0) return 1;
	if (value[Layer - 1][0]> 0 && value[Layer - 1][0]<= 0.5) return 2;
	if (value[Layer - 1][0] > 0.5 && value[Layer - 1][0]<= 1) return 3;
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
	cout << endl;
	cout << "Max Stap = " << maxStap << endl;
	cout << "Max Source = " << maxSource << endl;
	if(generation>0)
	cout << "Sr = " << sr[generation - 1] << endl;
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
		game[i].Input(max());
		game[i].Logic();
	}

}

void GA::Mutation() {
	srand(time(NULL));
	for (int i = 0 ; i <population; i++)
	{
		for (int j = 1; j < Layer; j++)
		{
			for (int k = 0; k < sizeLayer[j]; k++)
			{
				
				int p;
				p = rand() % sizeLayer[j - 1];
				weight[i][j][k][p] = (rand() % 200 * 0.01 - 1)/(generation+1);
			}
			int r = rand() % 1;
			if (r > 0)
				w0[i][j][j] += (rand() % 200 * 0.01 - 1)/(generation+1);
			
		}
	}
}

void GA::Save() {
	int x = generation, d;
	string filename = "GA";
	string num;
	char n;
	while (x!=0)
	{
		n = char(x % 10+48);
		x /= 10;
		num = n + num;
	}
	filename = filename + num + ".txt";

	ofstream f(filename);

	f << Layer << "\n";

	for (int i = 0; i < Layer; i++)
	{
		f << sizeLayer[i] << "\n";
	}

	for (int i = 1; i < Layer; i++)
	{
		f << w0[i] << " ";
		for (int j = 0; j < sizeLayer[i]; j++)
		{
			
			for (int k = 0; k < sizeLayer[i - 1]; k++)
			{
				f << weight[0][i][j][k] << " ";
			}
			f << "\n";
		}
	}
	f << "\nStapAlive = " << game[0].getStap();
	f << "\nSource = " << game[0].getSource();
	f << "\nSr = " << sr[generation];

	f.close();
}

void GA::Save(string filename) {

	ofstream f(filename);

	f << Layer << "\n";

	for (int i = 0; i < Layer; i++)
	{
		f << sizeLayer[i] << "\n";
	}

	for (int i = 1; i < Layer; i++)
	{
		
		for (int j = 0; j < sizeLayer[i]; j++)
		{
			f << w0[i][j] << " ";
			for (int k = 0; k < sizeLayer[i - 1]; k++)
			{
				f << weight[0][i][j][k] << " ";
			}
			f << "\n";
		}
	}
	f << "\nStapAlive = " << game[0].getStap();
	f << "\nSource = " << game[0].getSource();
	f << "\nSr = " << sr[generation];

}