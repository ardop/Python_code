#include "kinematics.h"
#include <fstream>
#include <armadillo>
#include <string>

using namespace std;

int main()
{
	while(1)
	{
		ifstream file("angles.txt");
		string line;
		cout << "starting" << endl;
		if(file.is_open())
		{
			while(getline(file, line))
			{
				cout << line << endl;
			}
			file.close();
		}
		else
		{
			cout << "Could not open file" << endl;
		}
	}
}
