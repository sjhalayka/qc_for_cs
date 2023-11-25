#include <iostream>
#include <complex>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXf;
using Eigen::MatrixXf;

int main(void)
{
	const int n = 6;

	VectorXf state_vector(n);
	state_vector <<
		6, 2, 1, 5, 3, 10;

	MatrixXf adjacency_matrix(n, n);
	adjacency_matrix << 
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 1,
		0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0,
		1, 0, 0, 0, 1, 0;

	const size_t tick_count = 3;

	for (size_t i = 0; i < tick_count; i++)
		state_vector = adjacency_matrix * state_vector;

	cout << state_vector << endl;

	return 0;
}