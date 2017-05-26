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
        
		
	while((pca1->error >= 0)  && getkey() != 27)
	{

		mat theta;
		mat zero;
		zero << 0;
		
		mat theta_sequence;
		
		mat target;
		target << -30 << 30 << 30;
		theta = calculate_ik_jacobian(target, false, zero);
		theta << 0;
		theta = kin_map_left(theta);
		
		
		theta_sequence = join_vert(theta_sequence, trans(theta));
		
		target.reset();
		target << -20 << 30 << 30;
		theta = calculate_ik_jacobian(target, false, zero);
		theta << 0;
		theta = kin_map_left(theta);
		
		theta_sequence = join_vert(theta_sequence, trans(theta));
		

		
		//rotate(val,pca1);
		execute_path_sequence(theta_sequence, 0, 0, pca1);
	}
	pca1->closePCA9685();
}
}
