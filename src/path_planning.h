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
	B = join_vert(theta_a, theta_b);
	
	// solving
	x = solve(A, B);

	// Verify the matrices by printing
	//print_check_matrix(A, B); 
	
	for(int i=n0;i<=nf;i++)
	{
		mat a;
		a << i << 1;
		
		mat theta_c;
		theta_c = a*x;
		
		// Appending to the matrix being returned
		configuration_history = join_vert(configuration_history, theta_c);
		

	}
	
	return configuration_history;
	
	
	
}

mat joint_path_cubic(const mat& theta_a, const mat& theta_b, int n0, int nf, double dq0, double dqf)
{
	mat configuration_history;

	// Time matrix
	mat A;
	A << pow(n0, 3) << pow(n0, 2) << n0 << 1 << endr << 3*pow(n0, 2) << 2*n0 << 1 << 0 << endr << pow(nf, 3) << pow(nf, 2) << nf << 1 << endr << 3*pow(nf, 2) << 2*nf << 1 << 0;

	// Function parameter matrix
	mat x;

	// Velocity vectors
	mat dq0_mat, dqf_mat;

	dq0_mat << dq0 << dq0 << dq0 << dq0 << dq0;
	dqf_mat << dqf << dqf << dqf << dqf << dqf;


	// Constraint matrix
	mat B;
	B = join_vert(join_vert(theta_a, dq0_mat), join_vert(theta_b, dqf_mat));

	x = solve(A, B);

	//print_check_matrix(A, B);

	for(int i=n0;i<=nf;i++)
	{
		mat a;
		a << pow(i, 3) << pow(i, 2) << i << 1;
		
		mat theta_c;
		theta_c = a*x;
		
		// Appending to the matrix being returned
		configuration_history = join_vert(configuration_history, theta_c);
	}

	return configuration_history;

}

// General joint path that makes the joints pass through all the given configurations

mat joint_path_general(const mat& theta_sequence, const mat& n_sequence, double dq0, double dqf)
{
	// This planning takes care of initial and final velocities and all the configurations in between
	
	mat configuration_history;
	
	//Time matrix. Must be generated dynamically as the degree of the equation used depends on the number of points
	// Defining degree of equation. It is equal to the number of constraints - 1. Number of constraints is number of 
	// theta points + 2 (for velocities)
	
	int constraints = theta_sequence.n_rows + 2;
	int positional_constraints = theta_sequence.n_rows;
	
	// Setting up the time matrix dynamically
	
	// First all the rows for position constraints
	mat A;
	for(int i=0;i<positional_constraints;i++)
	{
		mat tmp_A;
		for(int j=0;j<constraints;j++)
		{
			// Each row of A
			// Taking 'n' or iteration values from the n_sequence
			mat tmp_val;
			tmp_val << pow(n_sequence(i), constraints - j - 1);
			tmp_A = join_horiz(tmp_A, tmp_val);
		}
		
		A = join_vert(A, tmp_A);
	}
	
	// Adding the velocity constraints at the start and end positions
	
	for(int i=positional_constraints;i<positional_constraints + 2;i++)
	{
		mat tmp_A;
		for(int j=0;j<constraints;j++)
		{
			mat tmp_val;
			tmp_val << (constraints - j - 1)*pow(n_sequence(i), constraints - j - 2);
			tmp_A = join_horiz(tmp_A, tmp_val);
		}
		
		A = join_vert(A, tmp_A);
	}
	
	mat x;
	
	// Velocity vectors
	mat dq0_mat, dqf_mat;

	dq0_mat << dq0 << dq0 << dq0 << dq0 << dq0;
	dqf_mat << dqf << dqf << dqf << dqf << dqf;
	
	mat B;
	
	B = join_vert(theta_sequence, dq0_mat);
	B = join_vert(B, dqf_mat);
	
	x = solve(A, B);
	
	int n0 = n_sequence(0);
	int nf = n_sequence(n_sequence.n_rows - 1);

	print_check_matrix(A, B);
	
	for(int i=n0;i<=nf;i++)
	{
		mat a;
		
		for(int j=0;j<constraints;j++)
		{
			mat tmp_val;
			tmp_val << pow(i, constraints - j - 1);
			a = join_horiz(a, tmp_val);
		}
		
		mat theta_c;
		theta_c = a*x;
		
		configuration_history = join_vert(configuration_history, theta_c);
	}

	return configuration_history;
	
}
	
		

//mat joint_path_piecewise(mat const& theta_a, mat const& theta_b, int n0, int n1, int n2, int nf)
//{
	//mat configuration_history;

	//// Getting configurations in between the motion to stabilize the solutions
	//// A cubic equation is used for this purpose

	//mat A0;
	//A0 << pow(n0, 3) << pow(n0, 2) << n0 << 1 << endr << 3*pow(n0, 2) << 2*n0 << 1 << 0 << endr << pow(nf, 3) << pow(nf, 2) << nf << 1 << endr << 3*pow(nf, 2) << 2*nf << 1 << 0;

	//mat dq0_mat, dqf_mat;

	//// Setting initial velocity and final velocity to zero
	//dq0_mat << 0 << 0 << 0 << 0 << 0;
	//dqf_mat << 0 << 0 << 0 << 0 << 0;

	//mat B0;
	//B0 = join_vert(join_vert(theta_a, dq0_mat), join_vert(theta_b, dqf_mat));

	//mat x0;
	//x0 = solve(A0, B0);

	//// printing to check solution
	//print_check_matrix(A0, B0);

	//// Calculating configurations at n1 and n2
	//mat tmp_q1, tmp_q2, tmp_a1, tmp_a2;

	//tmp_a1 << pow(n1, 3) << pow(n1, 2) << n1 << 1;
	//tmp_a2 << pow(n2, 3) << pow(n2, 2) << n2 << 1;

	//tmp_q1 = tmp_a1*x0;
	//tmp_q2 = tmp_a2*x0;

	//// Calculating the velocity that makes the constant velocity phase and transition from and to rest possible
	//mat dqc;
	//dqc = (tmp_q1 - tmp_q2)/(n1-n2);

	//// Cubic equation from rest to constant velocity -------------------------------------------------------------------------------
	//mat A1;
	//A1 << pow(n0, 3) << pow(n0, 2) << n0 << 1 << endr << 3*pow(n0, 2) << 2*n0 << 1 << 0 << endr << pow(n1, 3) << pow(n1, 2) << n1 << 1 << endr << 3*pow(n1, 2) << 2*n1 << 1 << 0;

	//mat B1;
	//B1 = join_vert(join_vert(theta_a, dq0_mat), join_vert(tmp_q1, dqc));

	//mat x1;
	//x1 = solve(A1, B1);

	//// printing to check solution
	//print_check_matrix(A1, B1);

	//// Calculating constant of integrtion for the constant velocity phase (linear equation)
	//mat c1;
	//c1 = tmp_a1*x1;
	//c1 = c1 - (dqc*n1);

	//// Function parameters for the second phase ------------------------------------------------------------------------------------
	//mat x2;
	//x2 = join_vert(dqc, c1);

	//// Cubic function for motion to rest phase -------------------------------------------------------------------------------------

	//mat A3;
	//A3 << pow(nf, 3) << pow(nf, 2) << nf << 1 << endr << 3*pow(nf, 2) << 2*nf << 1 << 0 << endr << pow(n2, 3) << pow(n2, 2) << n2 << 1 << endr << 3*pow(n2, 2) << 2*n2 << 1 << 0;

	//mat B3;
	//B3 = join_vert(join_vert(theta_b, dqf_mat), join_vert(tmp_q2, dqc));

	//mat x3;
	//x3 = solve(A3, B3);

	//// printing to check solution
	//print_check_matrix(A3, B3);


	//// Trajectory is calculated in 3 phases. The trjectory history is updated in all three phases to form the combined path

	//for(int i=n0;i<=n1;i++)
	//{
		//mat a;
		//a << pow(i, 3) << pow(i, 2) << i << 1;

		//mat theta_c;
		//theta_c = a*x1;
		
		//// Appending to the matrix being returned
		//configuration_history = join_vert(configuration_history, theta_c);
	//}

	//for(int i=n1;i<=n2;i++)
	//{
		//mat a;
		//a << i << 1;

		//mat theta_c;
		//theta_c = a*x2;
		
		//// Appending to the matrix being returned
		//configuration_history = join_vert(configuration_history, theta_c);
	//}

	//for(int i=n2;i<=nf;i++)
	//{
		//mat a;
		//a << pow(i, 3) << pow(i, 2) << i << 1;

		//mat theta_c;
		//theta_c = a*x3;
		
		//// Appending to the matrix being returned
		//configuration_history = join_vert(configuration_history, theta_c);
	//}

	//return configuration_history;
//}

mat cartesian_path_straight_linear(mat const& theta_a, mat const& target_a, mat const& target_b, int n0, int nf)
{
	mat configuration_history;
	
	// zero mat
	mat zero;
	zero << 0;
	
	mat minus_one;
	minus_one << -1;
	
	mat t_a = theta_a;
	mat t_b = calculate_ik_jacobian(target_b, false, zero);
	t_b = join_vert(t_b, zero); // as the ik solution only gives 4 angles
	
	mat theta_b;
	
	if(t_b.n_rows == 2) // -1 and the appended 0
	{
		// Returns -1 if there is no solution
		cout << "No final point solution" << endl;
	}
	else
	{
		// Time matrix
		mat A;
		A << n0 << 1 << endr << nf << 1;
		
		// Constraint matrix
		mat B;
		B = join_vert(target_a, target_b);
		
		// Solving
		mat x;
		x = solve(A, B);
		
		cout << "Computing Trajectory" << endl;
		
		for(int i=n0;i<=nf;i++)
		{
			mat a;
			a << i << 1;
			
			mat target_c;
			target_c = a*x;
			
			// Computing the ik solutions for this target
			mat t_c;
			t_c = calculate_ik_jacobian(target_c, true, t_a);
			t_c = join_vert(t_c, zero); // As the ik solution only gives 4 angles
			
			cout << "Computing Trajectory" << endl;
			
			if(t_c.n_rows==2) // -1 and the appended 0
			{
				return minus_one;
			}
			
			
			
			
			configuration_history = join_vert(configuration_history, trans(t_c));
			
			t_a = trans(t_c);
			
		

		}
		
		cout << "Completed computing trajectory" << endl;
		
		mat tmpp = calculate_target(t_a);
		tmpp.print("final target");
	
		return configuration_history;
	}
	
}




























