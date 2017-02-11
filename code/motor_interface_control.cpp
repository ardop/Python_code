#include "motor_control.h"
#include "kinematics.h"
#include <fstream>
#include <armadillo>
#include <string>

using namespace std;

int main()
{
	double langles[5], rangles[5], hangles[2];
	
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
