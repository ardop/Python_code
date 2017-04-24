#include "motor_control.h"
#include <sstream>


using namespace std;

double str2double(string s) 
{  
    istringstream is(s);
    double n;
    is >> n;
	return n;
}

int main()
{
	int val[12];
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
        
        
        //Right hand
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
		
		int iter = 0;
		bool change = false; // Boolean to check if the motors need to be moved again
		mat previous_target;
		previous_target << 0 << 0 << 0;

		while((pca1->error >= 0 || pca1->error >= 0)  && getkey() != 27)
		{
			ifstream file("../code/ik_angles.txt");
			string line;
			double t_angle;
			mat target;
			target.reset();
			int count = 0;
			//cout << "starting" << endl;
			if(file.is_open())
			{
				while(getline(file, line))
				{
					t_angle = str2double(line);
					mat t_angle_mat;
					t_angle_mat << t_angle;
					target = join_horiz(target, t_angle_mat);
					count++;
					if(count==3)
						break;

				}	

					
			file.close();
			}
			else
			{
				cout << "Could not open file" << endl;
			}
			
			if(iter == 0)
			{
				// First iteration must store the target in stored_target
				previous_target(0) = target(0);
				previous_target(1) = target(1);
				previous_target(2) = target(2);
				change = true;
			}
			else
			{
				if(target(0)==previous_target(0) && target(1)==previous_target(1) && target(2)==previous_target(2))
				{
					// Both the angles are same. Dont move
					change = false;
				}
				else
				{
					change = true;
				}
				previous_target(0) = target(0);
				previous_target(1) = target(1);
				previous_target(2) = target(2);
			}
			
			if(change)
			{
			
				mat theta;
				mat zero;
				zero << 0;
				theta = calculate_ik_jacobian(target, false, zero);
				
				if(theta.n_rows == 1)
				{
					// If -1 is returned, then ignore as no solution is possible
					cout << "Ignore. No Solution" << endl;
				}
				else
				{

					val[0] = 53;
					val[1] = 150;
					val[2] = 105;
					val[3] = 0;
					val[4] = 90;
					val[5] = kin_map_left(1, theta(0));
					val[6] = kin_map_left(2, theta(1));
					val[7] = kin_map_left(3, theta(2));
					val[8] = kin_map_left(4, theta(3));
					val[9] = 90;
					val[10] = 90;
					val[11] = 90;
					
					cout << val[5] << " " << val[6] << " " << val[7] << " " << val[8] << endl;

					//target.print();
					//target.reset();
					cout << endl << endl;
					rotate(val,pca1);
				}
			}
			iter++;
		}
		pca1->closePCA9685();
	}
}
