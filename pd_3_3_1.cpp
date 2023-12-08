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

// See equation 3.26
MatrixXcf get_star_product(const int n, const MatrixXcf& m0, const MatrixXcf &m1)
{
	MatrixXcf ret(n, n);

	for (int i = 0; i < ret.cols(); i++)
	{
		for (int j = 0; j < ret.rows(); j++)
		{
			complex<float> sum = 0;

			for (int k = 0; k < n; k++)
				sum += m0(i, k) * m1(k, j);
			
			ret(i, j) = sum;
		}
	}

	return ret;
}


int main(void)
{
	//std::cout << std::fixed;
	//std::cout << std::setprecision(5);

	const int n = 8;
	const int k = 6;

	// See equation 3.50
	MatrixXcf P(n, n);

	const complex<float> u = 1.0f;
	const complex<float> a = u / sqrtf(2.0f);
	const complex<float> b = complex<float>(-1.0f,  1.0f) / sqrtf(6.0f);
	const complex<float> c = complex<float>(-1.0f, -1.0f) / sqrtf(6.0f);
	const complex<float> d = complex<float>( 1.0f, -1.0f) / sqrtf(6.0f);

	P <<
		0, 0, 0, 0, 0, 0, 0, 0,
		a, 0, 0, 0, 0, 0, 0, 0,
		a, 0, 0, 0, 0, 0, 0, 0,
		0, b, 0, u, 0, 0, 0, 0,
		0, c, 0, 0, u, 0, 0, 0,
		0, d, b, 0, 0, u, 0, 0,
		0, 0, c, 0, 0, 0, u, 0,
		0, 0, d, 0, 0, 0, 0, u;

	cout << "P = " << endl << P << endl << endl;

	// Get P^2
	// See equation	3.52
	cout << "P^2 = " << endl << get_star_product(n, P, P) << endl << endl;

	return 0;
}



/*
Output:

P =
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
		 (0.707107,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
		 (0.707107,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (-0.408248,0.408248)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (0.408248,-0.408248)  (-0.408248,0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (1,0)                 (0,0)
				(0,0)                 (0,0)  (0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (1,0)

P^2 =
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
 (-0.288675,0.288675)  (-0.408248,0.408248)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)                 (0,0)
				(0,0)  (0.408248,-0.408248)  (-0.408248,0.408248)                 (0,0)                 (0,0)                 (1,0)                 (0,0)                 (0,0)
(-0.288675,-0.288675)                 (0,0) (-0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (1,0)                 (0,0)
 (0.288675,-0.288675)                 (0,0)  (0.408248,-0.408248)                 (0,0)                 (0,0)                 (0,0)                 (0,0)                 (1,0)
*/