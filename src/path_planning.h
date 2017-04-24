#include <iostream>
#include <armadillo>
#include <math.h>
#include <stdio.h>
#include <string>

//Initializing pi, link lengths and constraints
#define PI 3.14159265358979323846
#define l1  14.2
#define l2  1.8
#define l3  14.5
#define l5  29.5
#define epsilon1 0.5235987756
#define epsilon2 0.1745329252
#define alpha2 0.5235987756

#define t1bl PI/2
#define t1al -PI/2
#define t2bl PI/2
#define t2al 0.0
#define t3bl PI
#define t3al 0.0
#define t4bl 0.0
#define t4al -PI/2
#define t5bl PI/2
#define t5al -PI/2


using namespace arma;
using namespace std;

// Defining standar mats
mat mat_zero;
mat_zero << 0;

void print_check_matrix(const mat& A, const mat& B)
{
	B.print("B: \n");
	
	mat x;
	x = solve(A, B);
	
	x.print("x: \n");
	
	cout << "---------------------------" << endl;
	
	mat temp = A*x;
	temp.print("A*x: \n");
	
	cout << "---------------------------" << endl;
	
}

mat joint_path_linear(const mat& theta_a, const mat& theta_b, int n0, int nf)
{
	mat configuration_history; // configuration history to be returned
	
	// Time matrix
	mat A;
	A << n0 << 1 << endr << nf << 1;
	
	// Function parameter matrix
	mat x;
	
	// Constraint matrix
	mat B;
	B = join_vert(theta_a.cols(1, 4), theta_b.cols(1, 4));
	
	// solving
	x = solve(A, B);

	// Verify the matrices by printing
	print_check_matrix(A, B); 
	
	for(int i=n0;i<=nf;i++)
	{
		mat a;
		a << i << 1;
		
		mat theta_c;
		theta_c = a*x;
		theta_c = join_horiz(theta_c, mat_zero);
		
		// Appending to the matrix being returned
		configuration_history = join_vert(configuration_history; theta_c);
	}
		
	
	return configuration_history;
	
	
	
}

mat joint_path_cubic(const mat& theta_a, const mat& theta_b, int n0, int nf, double dq0, double dqf)
{
	// Time matrix
	mat A;
	A << pow(n0, 3) << pow(n0, 2) << n0 << 1 << endr << 3*pow(n0, 2) << 2*n0 << 1 << 0 << endr << pow(nf, 3) << pow(nf, 2) << nf << 1 << endr << 3*pow(nf, 2) << 2*nf << 1 << 0;

	// Function parameter matrix
	mat x;

	// Velocity vectors
	mat dq0_mat, dqf_mat;

	dq0_mat << dq0 << dq0 << dq0 << dq0;
	dqf_mat << dqf << dqf << dqf << dqf;


	// Constraint matrix
	mat B;
	B = join_vert(join_vert(theta_a.cols(1, 4), dq0_mat), join_vert(theta_b.cols(1, 4), dqf_mat));

	x = solve(A, B);

	print_check_matrix(A, B);

	for(int i=n0;i<=nf;i++)
	{
		mat a;
		a << pow(i, 3) << pow(i, 2) << i << 1;
		
		mat theta_c;
		theta_c = a*x;
		theta_c = join_horiz(theta_c, mat_zero);
		
		// Appending to the matrix being returned
		configuration_history = join_vert(configuration_history; theta_c);
	}

	return configuration_history;

}


























