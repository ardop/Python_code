/*
 * The MIT License (MIT)

Copyright (c) 2015 Jetsonhacks

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
// servoExample.cpp
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
#include "kinematics.h"
#include "path_planning.h"

#define PI 3.14159265358979323846

using namespace std;

// Calibrated for a Robot Geek RGS-13 Servo
// Make sure these are appropriate for the servo being used!


int servoMin[14]={120, 135, 172, 155, 160, 150, 120, 135, 172, 155, 160, 150, 150, 150};
int servoMax[14]={612, 390, 659, 410, 675, 370, 612, 390, 659, 410, 675, 370, 650, 650};
int max_angle[14]={180, 90, 180, 90, 180, 90, 180, 90, 180, 90, 180, 90, 180, 180};

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

// Map an integer from one coordinate system to another
// This is used to map the servo values to degrees
// e.g. map(90,0,180,servoMin, servoMax)
// Maps 90 degrees to the servo value

double map ( int x, int in_min, int in_max, int out_min, int out_max) {
    double toReturn =  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min ;
    // For debugging:
    // printf("MAPPED %d to: %d\n", x, toReturn);
    return toReturn ;
}

double invmap(double y,double in_min,double in_max,double out_min,double out_max)
{
double toReturn = ((y-out_min)*(in_max-in_min)/(out_max-out_min)+in_min);
return toReturn;
}

double regread(int chan ,PCA9685 *a)
{
	double  read,read1,read2,dread;
	read1=a->getPWMofl(chan);
	usleep(100);
	read2=a->getPWMofh(chan);
	usleep(100);
	read=(read2*256)+read1;
	dread=invmap(read,0,max_angle[chan],servoMin[chan],servoMax[chan]);
	return dread;
}

void rotate(int d[14],PCA9685 *a)
{
    double j[14],temp[14],count[14],flag[14];   
	for(int i=0;i<14;i++)
	{
		temp[i]=round(regread(i,a));
		j[i]=temp[i];
		flag[i]=0;
		if(temp[i]>d[i])
		{
		count[i]=-1;
		}
		else if(temp[i]<d[i])
		{
		count[i]=1;
		}
		else if(temp[i]==d[i])
		count[i]=0;
	}
	
	while(true)
	{
		for(int i=0;i<14;i++)
		{
		temp[i]=round(regread(i,a));
		if(d[i]!=temp[i])
		{	
			j[i]+=count[i];
			a->setPWM(i,0,map(j[i],0,max_angle[i],servoMin[i],servoMax[i]));
		}
		else
		flag[i]=1;
		}
		for(int i=0;i<14;i++)
		{
		cout<<temp[i]<<"->"<<d[i]<<"..";
		}
		cout<<endl;
		usleep(5000);
		if(flag[0]==1 &&flag[1]==1 && flag[2]==1 && flag[3]==1 && flag[4]==1 && flag[5]==1 && flag[6]==1 && flag[7]==1 && flag[8]==1 && flag[9]==1 && flag[10]==1 && flag[11]==1 && flag[12]==1 && flag[13]==1)
			break;
	}
}

void execute_path(int d[14], mat const& target_b, int path_type, int model_type, PCA9685 *a)
{
	// computing theta_a and theta_b
	// The angles have to be inversed mapped to kinematic angles
	mat left_theta_a, left_theta_b, right_theta_a, right_theta_b, head_theta_a, head_theta_b;

	right_theta_b << d[0] << d[1] << d[2] << d[3] << d[4]; // right hand angles (configuration)
	left_theta_b << inv_kin_map_left(1, deg2rad(d[6])) << inv_kin_map_left(2, deg2rad(d[7])) << inv_kin_map_left(3, deg2rad(d[8])) << inv_kin_map_left(4, deg2rad(d[9])) << inv_kin_map_left(5, deg2rad(d[10])); // left hand angles (configuration)
	head_theta_b << d[12] << d[13];

	// Reading theta_a
	double temp_d[14];

	for(int i=0;i<14;i++)
	{
		temp_d[i]=round(regread(i,a));
	}

	right_theta_a << temp_d[0] << temp_d[1] << temp_d[2] << temp_d[3] << temp_d[4];
	left_theta_a << inv_kin_map_left(1, deg2rad(temp_d[6])) << inv_kin_map_left(2, deg2rad(temp_d[7])) << inv_kin_map_left(3, deg2rad(temp_d[8])) << inv_kin_map_left(4, deg2rad(temp_d[9])) << inv_kin_map_left(5, deg2rad(temp_d[10]));
	head_theta_a << temp_d[12] << temp_d[13];
	
	//left_theta_a.print();
	//left_theta_b.print();
	
	//cout << "----------------------------" << endl;
	
	//left_theta_a = kin_map_left(deg2rad(left_theta_a));
	//left_theta_b = kin_map_left(deg2rad(left_theta_b));
	
	//left_theta_a.print();
	//left_theta_b.print();
	
	//cout << "----------------------------" << endl;
	


	if(path_type == 0)
	{
		// Joint path
		if(model_type == 0)
		{
			// Linear
			mat configuration_history;

			int n0 = 0;
			int nf = 500;

			// The angles are converted to radians
			

			left_theta_a = deg2rad(left_theta_a);
			left_theta_b = deg2rad(left_theta_b);
			
			//left_theta_a.print();

			configuration_history = joint_path_linear(left_theta_a, left_theta_b, n0, nf);
			
			//configuration_history.print();

			// The angles must now be converted to degrees and mapped to pca angles before it is run
			for(int i=0;i<configuration_history.n_rows;i++)
			{
				mat tmp;
				tmp = kin_map_left(configuration_history.row(i));
				
				
				
				// Rounding off tmp
				tmp = round(tmp);
				
				// Move all the angles of the left arm to the specific angle value in this iteration
				for(int j=0;j<tmp.n_cols;j++)
				{
					a->setPWM(j+6, 0, map(tmp(j), 0, max_angle[j], servoMin[j], servoMax[j]));
				}
				
				// displaying movement
				for(int j=0;j<tmp.n_cols;j++)
				{
					cout << tmp(j) << "->" << d[j+6] << "..";
				}
				
				cout << endl;
				
				//pause
				usleep(5000);

			}
		}

		else if(model_type == 1)
		{
			// Cubic
			mat configuration_history;

			
			// Initial and final velocities
			double dq0 = 0.0;
			double dqf = 0.0;
			
			left_theta_a = deg2rad(left_theta_a);
			left_theta_b = deg2rad(left_theta_b);
			
			mat tmp_target_left_a = calculate_target(left_theta_a);
			mat tmp_target_left_b = calculate_target(left_theta_b);
			
			
			//for(int i=0;i<3;i++)
			//{
			
				//di = di + pow((tmp_target_left_a(i) - tmp_target_left_b(i)), 2);
			//}
			
			//di = sqrt(di);
			
			//double di = calculate_distance(trans(tmp_target_left_a), trans(tmp_target_left_b));
			
			// Computing angular distance between the initial and final angles to set the iteration count
			double di = calculate_distance(left_theta_a, left_theta_b);
			// Converting this angular distance in radians to degrees for better mapping to iteration count
			di = rad2deg(di);
			
			int n0 = 0;
			int nf = di * 6.25;
			
			
			//double distance = dist(trans(tmp_target_left_a, tmp_target_left_b));
			
			cout << "Distance: " << di << endl;

			configuration_history = joint_path_cubic(left_theta_a, left_theta_b, n0, nf, dq0, dqf);
			
			for(int i=0;i<configuration_history.n_rows;i++)
			{
				mat tmp;
				tmp = kin_map_left(configuration_history.row(i));
				
				// Rounding off tmp
				tmp = round(tmp);
				
				// Move all the angles of the left arm to the specific angle value in this iteration
				for(int j=0;j<tmp.n_cols;j++)
				{
					a->setPWM(j+6, 0, map(tmp(j), 0, max_angle[j], servoMin[j], servoMax[j]));
				}
				
				// displaying movement
				for(int j=0;j<tmp.n_cols;j++)
				{
					//cout << tmp(j) << "->" << d[j+6] << "..";
				}
				
				//cout << endl;
				
				//pause
				usleep(1000);
				
			}
		}

		//else if(model_type == 2)
		//{
			//// Piecewise
			//mat configuration_history;

			//int n0 = 0;
			//int n1 = 50;
			//int n2 = 950;
			//int nf = 1000;

			//configuration_history = joint_path_piecewise(left_theta_a, left_theta_b, n0, nf);
		//}

	}
	
	
	else if(path_type = 1)
	{
		//  Cartesian path
		
		if(model_type == 0)
		{
			// Straight line linear model
			
			mat configuration_history;

			int n0 = 0;
			int nf = 100;
			
			left_theta_a = deg2rad(left_theta_a);
			
			mat target_a = calculate_target(left_theta_a);

			configuration_history = cartesian_path_straight_linear(left_theta_a, trans(target_a), target_b, n0, nf);
			
			configuration_history.print("Configuration");
			
			if(configuration_history.n_rows == 1)
			{
				cout << "No intermediate solution" << endl; 
			}
			
			else
			{
			
				// Moving the motors
				
				for(int i=0;i<configuration_history.n_rows;i++)
				{
					mat tmp;
					tmp = kin_map_left(configuration_history.row(i));
					tmp.print("conf");
					
					// Rounding off tmp
					tmp = round(tmp);
					
					// Move all the angles of the left arm to the specific angle value in this iteration
					for(int j=0;j<tmp.n_cols;j++)
					{
						a->setPWM(j+6, 0, map(tmp(j), 0, max_angle[j], servoMin[j], servoMax[j]));
					}
					
					// displaying movement
					//for(int j=0;j<tmp.n_cols;j++)
					//{
						//cout << tmp(j) << "->" << d[j+6] << "..";
					//}
					
					
					//pause
					usleep(5000);
					
				}
			}
			
		}		
		
	}
}

void execute_path(mat const& theta_sequence, mat const& n_sequence, int path_type, int model_type, PCA9685 *a)
{
	
}



