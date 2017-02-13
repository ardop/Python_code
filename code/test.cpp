#include "kinematics.h"
#include "motor_control.h"


int main()
{
    PCA9685 *pca1 = new PCA9685(0x40);
    int err1 = pca1->openPCA9685();
    if (err1 < 0){
        printf("Error: %d", pca1->error);
    } else {
        printf("PCA9685 Device Address: 0x%02X\n",pca1->kI2CAddress);
        pca1->setAllPWM(0,0) ;
        pca1->reset() ;
        pca1->setPWMFrequency(50) ;
        // 27 is the ESC keyOC
        printf("Hit ESC key to exit\n");
        
        //Right hand
		pca1->setPWM(0,0,map(53,0,180,servoMin,servoMax));  //servo 1
		pca1->setPWM(1,0,map(150,0,180,servoMin,servoMax));  // servo 2
    	pca1->setPWM(2,0,map(105,0,180,servoMin,servoMax));  //servo 3
    	pca1->setPWM(3,0,map(0,0,180,servoMin,servoMax));  //servo 4
    	pca1->setPWM(4,0,map(90,0,180,servoMin,servoMax));  //servo 5
    	//Left hand
		pca1->setPWM(5,0,map(137,0,180,servoMin,servoMax)); //servo 1
		pca1->setPWM(6,0,map(26,0,180,servoMin,servoMax));  //servo 2
    	pca1->setPWM(7,0,map(105,0,180,servoMin,servoMax));  //servo 3
    	pca1->setPWM(8,0,map(180,0,180,servoMin,servoMax));  //servo 4
    	pca1->setPWM(9,0,map(90,0,180,servoMin,servoMax));  //servo 5
		//Head
		pca1->setPWM(10,0,map(90,0,180,servoMin,servoMax));  //up-down
		pca1->setPWM(11,0,map(90,0,180,servoMin,servoMax));  //sideways
    	
    	pca1->setPWM(12,0,map(180,0,180,servoMin,servoMax));  //sideways 	

		//for convenience : 53 150 105 0 90 137 26 105 180 90 90 90
		//default : 53 150 105 0 90 137 26 105 180 90 90 90

		int val[12];	
		while((pca1->error >= 0 || pca1->error >= 0)  && getkey() != 27)
		{    						
			for(int i=0;i<12;i++)
			{
				cin>>val[i];
			}
			rotate(val,pca1);
		}		
	}
	pca1->closePCA9685();
}
