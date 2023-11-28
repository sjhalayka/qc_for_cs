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
	adjacency_matrix <<
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
	// See equation 3.25 on page 84

	cout << endl;

	for (int i = 1; i <= k; i++)
		cout << "M^" << i << " = " << endl << get_mk_mul(n, i, adjacency_matrix) << endl << endl;

	return 0;
}


/*
Output:

	   0 0.166667 0.833333
0.333333      0.5 0.166667
0.666667 0.333333        0

0.166667 0.166667 0.666667

0.583333     0.25 0.166667
0.180556 0.347222 0.472222
0.451389   0.3125 0.236111
0.248843 0.346065 0.405093
0.395255 0.323495  0.28125
0.288291 0.340374 0.371335

M^1 =
	   0 0.166667 0.833333
0.333333      0.5 0.166667
0.666667 0.333333        0

M^2 =
 0.611111  0.361111 0.0277778
 0.277778  0.361111  0.361111
 0.111111  0.277778  0.611111

M^3 =
0.138889 0.291667 0.569444
0.361111 0.347222 0.291667
	 0.5 0.361111 0.138889

M^4 =
0.476852 0.358796 0.164352
0.310185 0.331019 0.358796
0.212963 0.310185 0.476852

M^5 =
0.229167 0.313657 0.457176
0.349537 0.336806 0.313657
0.421296 0.349537 0.229167

M^6 =
0.409337 0.347415 0.243248
0.321373 0.331211 0.347415
 0.26929 0.321373 0.409336

*/