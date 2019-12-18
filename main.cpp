#include "NetWork.h"
#include "GA.h"
//#include "Game.h"


int main() {
	char ch;
	srand(time(NULL));
	do {
		
		cout << "Choice algorithm BackPropogation(b) or GenenticAlgorithm(g) or Test Random(r)\n";
		cin >> ch;
		if (ch == 'b' || ch == 'B')
		{
			char a;
			cout << "Learning?(y/n)\n";
			cin >> a;
			if (a == 'y') {
				NetWork nw;
			}
			else if (a == 'n')
			{
				string filename;
				cout << "Enter filename when been weights: ";
				cin >> filename;
				NetWork nwr(filename);
			}

		}
		if (ch == 'g' || ch == 'G') {
			GA ga;
			//ga.Start();
		}

		if (ch == 'r') {
			int pop;
			cout << "Enter pop \n";
			cin >> pop;
			int* stap;
			int* source;
			stap = new int[pop];
			source = new int[pop];
			Game g;
			ofstream random("resultRandom.txt");
			for (int i = 0; i < pop; i++)
			{
				do
				{
					system("cls");
					g.Draw();
					int s = rand() % 3;
					cout << s << endl;
					g.Input(s);
					g.Logic();
					cout << g.getAlive() << endl;

				} while (g.getAlive());
				stap[i] = g.getStap();
				source[i] = g.getSource();
				random << i << ":\nStap = " << stap[i] << "\nSource = " << source[i]<<"\n";

				g.setAlive();

			}
			random.close();
			
			double sr = 0;
			for (int i = 0; i < pop; i++)
			{
				sr += stap[i];

			}
			sr /= pop;
			cout <<"Sr  = "<< sr << endl;
			_getch();
		}
		//delete nw;
		system("cls");
	} while (ch != 'e');
	return 0;
}