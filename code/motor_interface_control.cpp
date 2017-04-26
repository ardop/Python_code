#include "motor_control.h"
#include <sstream>

using namespace std;

int str2num(string s) 
{  
    istringstream is(s);
    int n;
    is >> n;
	return n;
}

int main()
{
	int val[14];
	PCA9685 *pca1 = new PCA9685(0x40);
    int err1 = pca1->openPCA9685();
    if (err1 < 0){
        printf("Error: %d", pca1->error);
    } else {
        printf("PCA9685 Device Address: 0x%02X\n",pca1->kI2CAddress);
        //pca1->setAllPWM(0,0) ;
        //pca1->reset() ;
        pca1->setPWMFrequency(60) ;
        // 27 is the ESC keyOC
        printf("Hit ESC key to exit\n");
        
        ////Right hand
		//pca1->setPWM(0,0,map(53,0,180,servoMin[0],servoMax[0]));  //servo 1
		//pca1->setPWM(1,0,map(150,0,180,servoMin[1],servoMax[1]));  // servo 2
    	//pca1->setPWM(2,0,map(105,0,180,servoMin[2],servoMax[2]));  //servo 3
    	//pca1->setPWM(3,0,map(0,0,180,servoMin[3],servoMax[3]));  //servo 4
    	//pca1->setPWM(4,0,map(90,0,180,servoMin[4],servoMax[4]));  //servo 5
    	////Left hand
		//pca1->setPWM(5,0,map(90,0,180,servoMin[5],servoMax[5])); //servo 1
		//pca1->setPWM(6,0,map(0,0,90,servoMin[6],servoMax[6]));  //servo 2
    	//pca1->setPWM(7,0,map(90,0,180,servoMin[7],servoMax[7]));  //servo 3
    	//pca1->setPWM(8,0,map(90,0,90,servoMin[8],servoMax[8]));  //servo 4
    	//pca1->setPWM(9,0,map(90,0,180,servoMin[9],servoMax[9]));  //servo 5
		////Head
		//pca1->setPWM(10,0,map(90,0,180,servoMin[10],servoMax[10]));  //up-down
		//pca1->setPWM(11,0,map(90,0,180,servoMin[11],servoMax[11]));  //sideways
		
	while((pca1->error >= 0)  && getkey() != 27)
	{
		ifstream file("../code/angles.txt");
		string line;
		int t_angle,count=0;
		cout << "starting" << endl;
		if(file.is_open())
		{
			while(getline(file, line))
			{
				t_angle = str2num(line);
				val[count]=t_angle;
				count++;
				if(count==14)
					break;
			}	
		file.close();
		}
		else
		{
			cout << "Could not open file" << endl;
		}
		rotate(val,pca1);
		//execute_path(val, 0, 0, pca1);
	}
	pca1->closePCA9685();
}
}
