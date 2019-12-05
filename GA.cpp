#include "GA.h"


GA::GA() {
	cout << "Enter number of poppulation\n";
	cin >> population;
	cout << "Enter number of radius\n";
	cin >> radius;
	this->radius = radius;
	game = new Game[population];
	neuron = new Neuron**[population];
	cout << "enter number of layer\n";
	size = new int[Layer];
	cin >> Layer;
	size[0] = pow(radius * 2 + 1, 2) -1;
	for (int i = 1; i < Layer-1; i++)
	{
		cout << "Enter size " << i << " layer\n";
		cin >> size[i];
	}
	size[Layer - 1] = 4;
	

	for (int  i = 0; i < population; i++)
	{
		neuron[i] = new Neuron*[Layer];
		for (int j = 0; j < Layer; j++)
		{
			neuron[i][j] = new Neuron[size[j]];
			for (int k = 0; k < size[j]; k++)
			{
				neuron[i][j][k].w0 = (rand() % 200 - 100) * 0.01;
				if (j < Layer - 1)
					for (int q = 0; q < size[j + 1]; q++)
					{
						neuron[i][j][k].weights[q] = (rand() % 200 - 100) * 0.01;
					}
			}

		}
	}
	input = new int[size[0]];
	generation = 0;
}


void GA::crooss() {


}

void GA::SortStep() {
	for (int i = 0; i < population; i++)
	{
		for (int j = 0; i < population-1; j++)
		{
			if ( game[j].getStap()< game[j+1].getStap()) {
				Game tmp;
				tmp	= game[i];
				game[i] = game[j];
				game[j] = tmp;
			}
		}
		
	}

}

void GA::SortSource() {
	int a = int(sqrt(population));
	Neuron** tmpN;
	Game tmp;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; i <a - 1; j++)
		{
			if (game[j].getSource() < game[j + 1].getSource()) {
				
				tmp = game[j+1];
				game[j+1] = game[j];
				game[j] = tmp;
				
				tmpN = neuron[j];
				neuron[j] = neuron[j + 1];
				neuron[j + 1] = tmpN;
				
			}
		}

	}
}