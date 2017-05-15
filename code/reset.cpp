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
        pca1->setAllPWM(0,0) ;
        pca1->reset() ;
        pca1->setPWMFrequency(60) ;
        // 27 is the ESC keyOC
		
        printf("Hit ESC key to exit\n");
       
	pca1->closePCA9685();
}
}
