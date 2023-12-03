#include <iostream>
#include <vector>
#include <complex>
#include <iomanip>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXcf;
using Eigen::MatrixXcf;


MatrixXcf get_mk_mul(const int n, const int k, const MatrixXcf& adjacency_matrix)
{
	if (k == 1)
		return adjacency_matrix;

	MatrixXcf ret(n, n);
	ret = adjacency_matrix;

	for (int i = 1; i < k; i++)
		ret = ret * adjacency_matrix;

	return ret;
}

MatrixXcf get_modulus_squared(const int n, const MatrixXcf& m)
{
	MatrixXcf ret(n, n);

	for (int i = 0; i < ret.cols(); i++)
		for (int j = 0; j < ret.rows(); j++)
			ret(i, j) = pow(abs(m(i, j)), 2.0f);

	return ret;
}


int main(void)
{
	//std::cout << std::fixed;
	//std::cout << std::setprecision(5);

	const int n = 8;
	const int k = 6;


	// See equation 3.50
	MatrixXcf adjacency_matrix(n, n); // aka M^1

	const complex<float> u = 1.0f;
	const complex<float> a = u / sqrtf(2.0f);
	const complex<float> b = complex<float>(-1.0f,  1.0f) / sqrtf(6.0f);
	const complex<float> c = complex<float>(-1.0f, -1.0f) / sqrtf(6.0f);
	const complex<float> d = complex<float>( 1.0f, -1.0f) / sqrtf(6.0f);

	adjacency_matrix <<
		0, 0, 0, 0, 0, 0, 0, 0,
		a, 0, 0, 0, 0, 0, 0, 0,
		a, 0, 0, 0, 0, 0, 0, 0,
		0, b, 0, u, 0, 0, 0, 0,
		0, c, 0, 0, u, 0, 0, 0,
		0, d, b, 0, 0, u, 0, 0,
		0, 0, c, 0, 0, 0, u, 0,
		0, 0, d, 0, 0, 0, 0, u;

	cout << adjacency_matrix << endl << endl;

	VectorXcf state_vector(n); // aka X
	state_vector <<
		1, 1, 1, 1, 1, 1, 1, 1;

	cout << state_vector.transpose() << endl << endl;

	for (int i = 1; i <= k; i++)
	{
		state_vector = adjacency_matrix * state_vector; // aka Y = MX
		cout << state_vector.transpose() << endl;
	}


	// Get M^k path probability, where k = 1 through 6
	cout << endl;

	for (int i = 1; i <= k; i++)
		cout << "M^" << i << " = " << endl << get_mk_mul(n, i, adjacency_matrix) << endl << endl;

	return 0;
}



/*
Output:

				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
		 (0.707107,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
		 (0.707107,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (-0.408248,0.408248)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (0.408248,-0.408248)  (-0.408248,0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (1,0)                 (0,0)
				(0,0)                 (0,0)  (0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (1,0)

(1,0) (1,0) (1,0) (1,0) (1,0) (1,0) (1,0) (1,0)

			   (0,0)         (0.707107,0)         (0.707107,0)  (0.591752,0.408248) (0.591752,-0.408248)                (1,0) (0.591752,-0.408248)  (1.40825,-0.408248)
			   (0,0)                (0,0)                (0,0)  (0.303077,0.696923) (0.303077,-0.696923)                (1,0) (0.303077,-0.696923)  (1.69692,-0.696923)
			   (0,0)                (0,0)                (0,0)  (0.303077,0.696923) (0.303077,-0.696923)                (1,0) (0.303077,-0.696923)  (1.69692,-0.696923)
			   (0,0)                (0,0)                (0,0)  (0.303077,0.696923) (0.303077,-0.696923)                (1,0) (0.303077,-0.696923)  (1.69692,-0.696923)
			   (0,0)                (0,0)                (0,0)  (0.303077,0.696923) (0.303077,-0.696923)                (1,0) (0.303077,-0.696923)  (1.69692,-0.696923)
			   (0,0)                (0,0)                (0,0)  (0.303077,0.696923) (0.303077,-0.696923)                (1,0) (0.303077,-0.696923)  (1.69692,-0.696923)

M^1 =
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
		 (0.707107,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
		 (0.707107,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (-0.408248,0.408248)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (0.408248,-0.408248)  (-0.408248,0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (1,0)                 (0,0)
				(0,0)                 (0,0)  (0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (1,0)

M^2 =
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
 (-0.288675,0.288675)  (-0.408248,0.408248)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (0.408248,-0.408248)  (-0.408248,0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675)                 (0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (1,0)                 (0,0)
 (0.288675,-0.288675)                 (0,0)  (0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (1,0)

M^3 =
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
 (-0.288675,0.288675)  (-0.408248,0.408248)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (0.408248,-0.408248)  (-0.408248,0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675)                 (0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (1,0)                 (0,0)
 (0.288675,-0.288675)                 (0,0)  (0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (1,0)

M^4 =
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
 (-0.288675,0.288675)  (-0.408248,0.408248)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (0.408248,-0.408248)  (-0.408248,0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675)                 (0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (1,0)                 (0,0)
 (0.288675,-0.288675)                 (0,0)  (0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (1,0)

M^5 =
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
 (-0.288675,0.288675)  (-0.408248,0.408248)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (0.408248,-0.408248)  (-0.408248,0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675)                 (0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (1,0)                 (0,0)
 (0.288675,-0.288675)                 (0,0)  (0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (1,0)

M^6 =
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
 (-0.288675,0.288675)  (-0.408248,0.408248)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (0.408248,-0.408248)  (-0.408248,0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675)                 (0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (1,0)                 (0,0)
 (0.288675,-0.288675)                 (0,0)  (0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (1,0)
*/