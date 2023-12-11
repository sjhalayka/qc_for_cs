#include <iostream>
#include <vector>
#include <complex>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXf;
using Eigen::MatrixXf;


MatrixXf get_mk_mul(const int n, const int k, const MatrixXf& adjacency_matrix)
{
	if (k == 1)
		return adjacency_matrix;

	MatrixXf ret(n, n);
	ret = adjacency_matrix;

	for (int i = 1; i < k; i++)
		ret = ret * adjacency_matrix;

	return ret;
}


int main(void)
{
	const int n = 3;

	MatrixXf adjacency_matrix(n, n); // aka M^1
	adjacency_matrix << // See equation 3.14
		0,           1.0f / 6.0f, 5.0f / 6.0f,
		1.0f / 3.0f, 1.0f / 2.0f, 1.0f / 6.0f,
		2.0f / 3.0f, 1.0f / 3.0f, 0; 

	cout << adjacency_matrix << endl << endl;

	VectorXf state_vector(n); // aka X
	state_vector <<
		1.0f / 6.0f, 1.0f / 6.0f, 2.0f / 3.0f;

	cout << state_vector.transpose() << endl << endl;

	const size_t k = 6;

	for (int i = 1; i <= k; i++)
	{
		state_vector = adjacency_matrix * state_vector; // aka Y = MX
		cout << state_vector.transpose() << endl;
	}

	// Get M^k path probability, where k = 1 through 6
	// See equations 3.22 and 3.25

	cout << endl;

	for (int i = 1; i <= k; i++)
		cout << "M^" << i << " = " << endl << get_mk_mul(n, i, adjacency_matrix) << endl << endl;

	return 0;
}

