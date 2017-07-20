#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <termios.h>
#include <time.h>
#include <JHPWMPCA9685.h>
#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

 int servoMin = 160;
 int servoMax = 600;
 
 int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    /* set the terminal to raw mode */
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    /* read a character from the stdin stream without blocking */
    /*   returns EOF (-1) if no character is available */
    character = fgetc(stdin);

    /* restore the original terminal attributes */
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}

double map ( int x, int in_min, int in_max, int out_min, int out_max) {
    double toReturn =  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min ;
    // For debugging:
     //printf("MAPPED %d to: %d\n", x, toReturn);
    return toReturn ;
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
        pca1->setAllPWM(0,0) ;
        pca1->reset() ;
        pca1->setPWMFrequency(60) ;
        // 27 is the ESC keyOC
        printf("Hit ESC key to exit\n");
        
		//pca1->setPWM(1,0,servoMin);  //servo 1
		 pca1->setPWM(1,0,servoMax);
		//pca1->setPWM(1,0,map(90,0,180,servoMin,servoMax));
	pca1->closePCA9685();
}
}
