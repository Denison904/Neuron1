#include "NetWork.h"
#include "GA.h"
//#include "Game.h"


int main() {
	char ch;
	cout << "Choice algorithm BackPropogation(b) or GenenticAlgorithm(g)\n";
	cin >> ch;
	if (ch == 'b'||ch=='B')
	{
		char a;
		cout << "Learning?(y/n)\n";
		cin >> a;
		if (a == 'y') {
			vector<NetWork> nw;
			for (int i = 0; i < 3; i++)
			{
				nw.push_back(NetWork());	
			}
	
		}
		else if (a=='n')
		{
			string filename;
			cout << "Enter filename when been weights: ";
			cin >> filename;
			NetWork nwr(filename);
		}
	
	}
	if (ch == 'g'||ch=='G') {
		GA ga;
		//ga.Start();
	}


	//delete nw;
	
	return 0;
}