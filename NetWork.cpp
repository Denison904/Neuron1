#include "NetWork.h"
//sigm or tanh
#define sigm
//#define tanh

NetWork::NetWork() {
	cout << "Enter radius\n";
	cin >> radius;
	cout << "Enter number of layers\n";
	cin >> Layer;
	size= new int[Layer];
	
	srand(time_t(NULL));
	size[0] = pow(radius*2+1,2)-1;
	
	for (int i = 1; i < Layer-1; i++)
	{
		cout << "Enter size " << i << " layer\n";
		cin >> size[i];
	}
	

	size[Layer - 1] = 4;
	
	cout << "Enter number of snake \n";
	cin >> population;
	game =new Game[population+2];
	for (int i = 0; i < population; i++)
	{
		game[i] = Game();
	}

	data = new Data*[population];
	for (int i = 0; i < population; i++)
	{
		data[i] = new Data[50];
	}
	//number = 0;
	stapAlive = new int[population];

	cout << "Enter learning rate:\n";
	cin >> lr;
	//lr = 0.1;
	cout << "Enter step:\n";
	cin >> h;
	
	neuron = new Neuron * [Layer];
	for (int  i = 0; i < Layer; i++)
	{
		neuron[i] = new Neuron[size[i]];
		
		for (int j = 0; j < size[i]; j++)
		{
			neuron[i][j].w0 = (rand() % int(2.0/h))* h - 1;
			if (i > 0) {
				neuron[i][j].weights = new double[size[i - 1]];	
				neuron[i][j].error = 0;
				for (int k = 0; k < size[i - 1]; k++)
				{
					neuron[i][j].weights[k] = (rand() %int(2.0/h))*h - 1 ;
				}
			}
		}
	}
	Start();
}

NetWork::NetWork(string filename) {
	fstream f;
	f.open(filename);
	if (!f.is_open())
	{
		cout << "File isn't open!" << endl;
		return;
	}
	f>>Layer;
	size = new int[Layer];
	for (int i = 0; i <Layer ; i++)
	{
		f >> size[i];
	}
	neuron = new Neuron *[Layer];
	for (int i = 0; i < Layer; i++)
	{
		neuron[i] = new Neuron[size[i]];
		for (int j = 0; j < size[i]; j++)
		{
			if (i>0)
			{
				f >> neuron[i][j].w0;
				neuron[i][j].weights = new double[size[i - 1]];
				for (int k = 0; k < size[i-1]; k++)
				{
					f >> neuron[i][j].weights[k];
				}
			}
		}
	}
	Start();
}

void NetWork::Sensor(int number) {
	int *coor;
	coor = game[number].getCoord();
	for (int i = 0; i < 2; i++)
	{
		cout << coor[i]<<endl;
	}
	coor[0] -= radius;
	coor[1] -= radius;
	int k = 0;
	for (int i = 0; i < 2*radius+1; i++)
	{
		for (int j = 0; j < 2*radius+1; j++)
		{
			if ((i != radius || j!=radius)) {
				if (game[number].CheckBorder(coor[0] + j, coor[1] + i)) 
				{
					neuron[0][k++].value = -1;
				}
				else if(game[number].CheckTail(coor[0]+j, coor[1]+i))\
				{
					neuron[0][k++].value = -1;
				}
				else if (game[number].CheckFruit(coor[0]+j,coor[1]+i))
				{
					neuron[0][k++].value = 1;
				}
				else
				{
					neuron[0][k++].value = 0;
				}
			}
		}
	}
}


void NetWork::ForwardFeed() {
	for (int i = 1; i < Layer; i++)
	{
		for (int j = 0; j < size[i]; j++)
		{
			double arg = 0;
			for (int k = 0; k < size[i-1]; k++)
			{
				arg += neuron[i-1][k].value * neuron[i][j].weights[k];
			}
			neuron[i][j].value = act(arg+neuron[i][j].w0);
		}
	}
}



double NetWork::act(double x) {
#ifdef sigm
	return 2.0 / (1 + exp(-x)) -1;
#endif // sigm
#ifdef tanh
	return tanh(x);
#endif 
}

double NetWork::Dact(double x) {
#ifdef sigm
	return 2.0* act(x) * (1.0 - act(x));
#endif // 
#ifdef tanh
	return (1+act(x))/(1-act(x));
#endif // tanh
}

int NetWork::max() {
	int index = 0;
	int max = neuron[Layer - 1][0].value;
	for (int i = 1; i < size[Layer-1]; i++)
	{
		if (neuron[Layer - 1][i].value > max) {
			index = i;
			max = neuron[Layer - 1][i].value;
		}
	}
	return index;
	if (neuron[Layer - 1][0].value >= 0.0 && neuron[Layer - 1][0].value <= 0.25) return 0;
	if (neuron[Layer - 1][0].value > 0.25 && neuron[Layer - 1][0].value <= 0.25*2) return 1;
	if (neuron[Layer - 1][0].value > 0.25*2 && neuron[Layer - 1][0].value <= 0.25*3) return 2;
	if (neuron[Layer - 1][0].value > 0.25*3 && neuron[Layer - 1][0].value <= 1) return 3;

}

void NetWork::BackPropogation( double *rr ) {
	for (int i = 0; i < size[Layer-1]; i++)
	{
		double arg = 0;
		for (int j = 0; j < size[Layer-2]; j++)
		{
			arg += neuron[Layer - 2][j].value * neuron[Layer - 1][i].weights[j];
		}
		neuron[Layer - 1][i].error = pow(rr[i] - neuron[Layer-1][i].value, 2) * Dact(neuron[Layer - 1][i].w0 + arg);
	}

	for (int i = Layer-2; i > 0; i--)
	{
		for (int j = 0; j < size[i]; j++)
		{
			neuron[i][j].error = 0;
			double arg = 0;
			for (int k = 0; k < size[i + 1]; k++)
			{
				neuron[i][j].error += neuron[i + 1][k].error * neuron[i+1][k].weights[j];
			}
			for (int k = 0; k < size[i-1]; k++)
			{
				arg += neuron[i-1][k].value * neuron[i][k].weights[j];
			}
				
	
			neuron[i][j].error = Dact(arg+neuron[i][j].w0) * neuron[i][j].error;
		}
	}

	for (int i = 1; i < Layer; i++)
	{
		//dw[i] = new double* [size[i]];
		for (int j = 0; j < size[i]; j++)
		{
			//dw[i][j] = new double[size[i + 1]];
			for (int k = 0; k < size[i-1]; k++)
			{
				//dw[i][j][k]
				neuron[i][j].weights[k] += neuron[i][j].value * neuron[i][k].error;
			}
			neuron[i][j].w0 = neuron[i][j].error;			
		}
	}



}



void NetWork::Start() {
	saveWeights();
	for (int i = 0; i < population; i++)
	{
		int j = 0;
		while (game[i].getAlive())
		{
			Sensor(i);
			ForwardFeed();
			Draw(i);
			game[i].Input(InputRR());
			Save(i,j);
			game[i].Logic();

			j++;
		} 
		stapAlive[i] = j-1;
	}
	/*
	for (int i = 0; i < stapAlive[0]; i++)
	{
		
		BookLearning(data[0][i].rr, data[0][i].predict);
		for (int i = 0; i < size[0]; i++)
		{
			neuron[0][i].value = data[0][i].input[i];
		}
		ForwardFeed();
		double testRR[4];
		for (int i = 0; i < 4; i++)
		{
			testRR[i] = neuron[Layer - 1][i].value;
		}
		cout << "Delta = " << delta(data[0][i].rr, testRR) << endl;
	}
	saveWeights();
	char a;
	cin >> a;
	
	while (game[population].getAlive())
	{
		char  a;
		Sensor(population);
		ForwardFeed();
		
		Draw(population);
		a = _getch();
		game[population].Input(max());
		game[population].Logic();
		
	}*/
	char contin ='y';
	int iter=0;
	double** tmp;
	tmp = new double* [population];
	for (int i = 0; i < population; i++)
	{
		tmp[i] = new double[stapAlive[i]];
	}
	do
	{
		for (int i = 0; i < population; i++)
		{		
			for (int j = 0; j < stapAlive[i]; j++)
			{
				for (int k = 0; k < size[0]; k++)
				{
					neuron[0][k].value = data[i][j].input[k];
				}
				ForwardFeed();
				double* tmpOutput;
				tmpOutput = new double[size[Layer - 1]];
				for (int i = 0; i < size[Layer-1]; i++)
				{
					tmpOutput[i] = neuron[Layer - 1][i].value;
				}
				tmp[i][j] = delta(tmpOutput, data[i][j].rr);
				BackPropogation(rr);
				//BookLearning();
				delete[] tmpOutput;
			}
		}
		lr = lr / 2;
		double m = 0;
		for (int i = 0; i < population; i++)
		{
			for (int j = 0; j < stapAlive[i]; j++)
			{
				if (abs(tmp[i][j]) > abs(m))
					m = tmp[i][j];
			}
		}
		if (iter%10 == 0)
		{
			game[population] = Game();
			int j = 0;
			while (game[population].getAlive())
			{
				Sensor(population);
				ForwardFeed();
				Draw(population);
				game[population].Input(max());
				game[population].Logic();

				j++;
			}
			stapAlive[population] = j - 1;

			cout << "Delta:\t" << m << endl;
			cout << "iter:\t" << iter << endl;
			cout << "Continue?\n";
			cin >> contin;
		}
		iter++;
	} while (contin == 'y');


	while (game[population+1].getAlive())
	{
		char  a;
		Sensor(population+1);
		ForwardFeed();

		Draw(population+1);
		game[population+1].Input(max());
		game[population+1].Logic();
		a = _getch();
	}


	cout << "Do you want to save weighst?(y/n)";
	char answ;
	cin >> answ;
	if (answ == 'y')
		saveWeights();
	

	for (int i = 0; i < population; i++)
	{
		delete[] tmp[i];
	}
	delete[] tmp;
}


void NetWork::Draw(int n) {
	if (!game[n].getAlive())
		return;
	int k = 0;
	system("cls");
	game[n].Draw();
	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
			if ((i != 0 || j != 0))
				cout << neuron[0][k++].value<<"\t";
			else
				cout << "\t\t";
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < size[Layer-1]; i++)
	{
		cout<<neuron[Layer - 1][i].value<<"\t";
	}
	cout << endl;
	for (int i = 0; i < size[Layer-1]; i++)
	{	
		cout << neuron[Layer - 1][i].w0;
		for (int j = 0; j < size[Layer-2]; j++)
		{
			cout<<neuron[Layer - 1][i].weights[j]<<"\t";
		}
		cout << "\n";
	}
	
}

int NetWork::InputRR() {
	char a;
	a = _getch();
	switch (a)
	{
	case 'A':case 'a':
		for (int i = 0; i < 4; i++)
		{
			
			
			if (i == 0) {
				rr[i] = 1;
			}
			else
			{
				rr[i] = 0;
			}
			
		}
		//rr[0] = 0.125;
		return 0;
		break;
	case 'W':case 'w':
		for (int i = 0; i < 4; i++)
		{
			if (i == 1) {
				rr[i] = 1;
			}
			else
			{
				rr[i] = 0;
			}
		}
		//rr[0] = 0.125*3;
		return 1;
		break;
	case 'D':case 'd':
		for (int i = 0; i < 4; i++)
		{
			if (i == 2) {
				rr[i] = 1;
			}
			else
			{
				rr[i] = 0;
			}
		}
		//rr[0] = 0.125*5;
		return 2;
		break;
	case 'S':case 's':
		for (int i = 0; i < 4; i++)
		{
			if (i == 3) {
				rr[i] = 1;
			}
			else
			{
				rr[i] = 0;
			}
		}
		//rr[0] = 0.125*7;
		return 3;
		break;
	default:
		break;
	}

}

void NetWork::Save(int i, int j) {
	double* tmpI;
	tmpI = new double[size[0]];
	double tmpO[4];
	for (int i = 0; i < 4; i++)
	{
		tmpI[i] = neuron[Layer - 1][i].value;
	}

	for (int i = 0; i < size[0]; i++)
	{
		tmpI[i] = neuron[0][i].value;
	}


	data[i][j] = Data(tmpI,tmpO , rr);
	delete[] tmpI;
}

void NetWork::saveWeights() {
	string filename;
	cout << "Enter filename: ";
	cin >> filename;
	ofstream f(filename);

	f << Layer<<"\n";

	for (int i = 0; i < Layer; i++)
	{
		f << size[i] << "\n";
	}

	for (int i = 1; i < Layer; i++)
	{
		for (int j = 0; j < size[i]; j++)
		{
			f << neuron[i][j].w0<<" ";
			for (int k = 0;  k < size[i-1]; k++)
			{
				f << neuron[i][j].weights[k]<<" ";
			}
			f << "\n";
		}
	}
	f.close();
}

double NetWork::delta(double *output, double *rr) {
	double max = rr[0]- output[0];
	for (int i = 1; i < size[Layer-1]; i++)
	{
		if (abs(max) < abs(rr[i] - output[i]))
			max = rr[i] - output[i];
	}
	return max;
}

void NetWork::BookLearning(double *rr, double *predict) {
	
	for (int i = 0; i < size[Layer-1]; i++)
	{
		neuron[Layer - 1][i].error = pow(predict[i] - rr[i],2);
	}
	for (int i = 0; i < size[Layer-1]; i++)
	{
		for (int j = 0; j < size[Layer-2]; j++)
		{
			neuron[Layer - 1][i].weights[j] -= lr*dif(neuron[Layer - 1][i].error);

		}
		//neuron[Layer - 1][i].w0 -= dif(neuron[Layer - 1][i].error);
		neuron[Layer - 1][i].w0 = 0;
	}
	
}

double NetWork::dif(double x) {
	return (act(x - 2 * h) - 8 * act(x - h) + 8 * act(x + h) + act(x + 2 * h)) / 2;
}
