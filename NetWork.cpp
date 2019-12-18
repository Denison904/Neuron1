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
	save = new double* [20];
	size[Layer - 1] = 1;
	rr = new double [20];

	game = new Game[2];
	for (int i = 0; i < 2; i++)
	{
		game[i] = Game();
	}

	cout << "Enter learning rate:\n";
	cin >> lr;
	//lr = 0.1;
	cout << "Enter step:\n";
	cin >> h;
	w0 = new double* [Layer];
	value = new double* [Layer];
	weights = new double** [Layer - 1];
	for (int i = 0; i < Layer; i++)
	{
		w0[i] = new double[size[i]];
		weights[i] = new double*[size[i]];
		value[i] = new double[size[i]];
		if(i>0)
		for (int j = 0; j < size[i]; j++)
		{
			w0[i][j] = rand() % 200 * 0.01 - 1;
			weights[i][j] = new double[size[i - 1]];
			for (int k = 0; k < size[i-1]; k++)
			{
				weights[i][j][k] = rand() % 200 * 0.01 - 1;
			}
		}
	}
	rr = new double[1];
	dw = new double** [Layer];
	for (int i = 1; i < Layer; i++)
	{
		dw[i] = new double* [size[i]];
		for (int j = 0; j < size[i]; j++)
		{
			dw[i][j] = new double[size[i - 1]];
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
	
	Start();
}

void NetWork::Sensor(int number) {
	int *coor;
	coor = game[number].getCoord();
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
					value[0][k++] = -1;
				}
				else if(game[number].CheckTail(coor[0]+j, coor[1]+i))\
				{
					value[0][k++] = -1;
				}
				else if (game[number].CheckFruit(coor[0]+j,coor[1]+i))
				{
					value[0][k++] = 1;
				}
				else
				{
					value[0][k++] = 0;
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
			double arg = w0[i][j];
			for (int k = 0; k < size[i-1]; k++)
			{
				arg += value[i - 1][k] * weights[i][j][k];
			}
			value[i][j] = act(arg);
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
	if (value[Layer-1][0] >= -1 && value[Layer-1][0]<= -0.5) return 0;
	if (value[Layer-1][0]> -0.5 && value[Layer-1][0]<= 0) return 1;
	if (value[Layer-1][0]> 0 && value[Layer-1][0]<= 0.5) return 2;
	if (value[Layer-1][0]> 0.5 && value[Layer-1][0]<= 1) return 3;
}

void NetWork::BackPropogation( double rr ) {
	double E = 0;
	double arg = w0[1][0];
	for (int i = 0; i < size[Layer-2]; i++)
	{
		
		for (int j = 0; j < size[Layer-1]; j++)
		{
			arg += weights[1][j][i] * value[0][i];
		}
		E -= (rr - act(arg))*(rr - act(arg));
	}

	E /= size[0];

	for (int i = 1; i < Layer; i++)
	{
		for (int j = 0; j < size[i]; j++)
		{
			for (int k = 0; k < size[i-1]; k++)
			{
				weights[i][j][k] -= lr * value[0][j] * 4 * (rr - act(arg))*Dact(arg);
			}
			w0[i][j]-=lr*4* rr;
		}
	}
		

}

double  NetWork::norma(double*** dw, double ***w) {
	double sum = 0;
	for (int i = 1; i <Layer; i++)
	{
		
		for (int j = 0; j < size[i]; j++)
		{
			for (int k = 0; k < size[i-1]; k++)
			{
				sum += abs(dw[i][j][k]-w[i][j][k]);
			}
		}
	}
	return sum;

}


void NetWork::Start() {
	int stap = 0;
	do
	{
		Sensor(0);
		ForwardFeed();
		Draw(0);
		
		save[stap] = value[0];
		game[0].Input(InputRR(stap));
		game[0].Logic();

		if (stap >= 20) break;
		stap++;

	} while (game[0].getAlive());
	do
	{	dw = weights;
		dw0 = w0;
		for (int i = 0; i < 20; i++)
		{
			
			value[0] = save[i];
			ForwardFeed();
			BackPropogation(rr[i]);
			
		}
		cout << "Norma: " << norma(dw , weights) << endl;
	} while (norma(dw, weights)>h);
	char a;
	cin>>a;
	game[0] = Game();
	do
	{
		Sensor(1);
		ForwardFeed();
		Draw(1);
		game[1].Input(max());
		game[1].Logic();
	} while (game[1].getAlive());
	
}

int NetWork::InputRR(int i) {
	char a;
	a = _getch();
	switch (a)
	{
	case 'A':case 'a':
		rr[i] = -0.75;
		return 0;
		break;
	case 'W':case 'w':
		rr[i] = -0.25;
		return 1;
		break;
	case 'D':case 'd':
		rr[i] = 0.25;
		return 2;
		break;
	case 'S':case 's':
		rr[i] = 0.75;
		return 3;
		break;
	default:
		break;
	}

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
			f << w0[i][j]<<" ";
			for (int k = 0;  k < size[i-1]; k++)
			{
				f << weights[i][j][k]<<" ";
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
	
	lr /= 2;
	
}

double NetWork::dif(double x) {
	return (act(x - 2 * h) - 8 * act(x - h) + 8 * act(x + h) + act(x + 2 * h)) / 2;
}



void NetWork::Draw(int i)
{
	if (!game[i].getAlive())
		return;
	int k = 0;
	system("cls");
	game[i].Draw();


}