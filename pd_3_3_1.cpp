#include <iostream>
#include <vector>
#include <complex>
#include <iomanip>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXcf;
using Eigen::MatrixXcf;


MatrixXcf get_modulus_squared(const int n, const MatrixXcf& m)
{
	MatrixXcf ret(n, n);

	for (int i = 0; i < ret.cols(); i++)
		for (int j = 0; j < ret.rows(); j++)
			ret(i, j) = pow(abs(m(i, j)), 2.0f);

	return ret;
}

// This is the same as the regular matrix product from Eigen
// Is that how it's supposed to be?
MatrixXcf get_star_product(const int n, const MatrixXcf& m0, const MatrixXcf &m1)
{
	MatrixXcf ret(n, n);

	for (int i = 0; i < ret.cols(); i++)
	{
		for (int j = 0; j < ret.rows(); j++)
		{
			complex<float> sum = 0;

			for (int k = 0; k < n; k++)
				sum += m0(i, k) * m1(k, j); // See equation 3.26
			
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

	// See equation 3.50
	MatrixXcf P(n, n);

	const complex<float> u = complex<float>( 1.0f,  0.0f);
	const complex<float> a = complex<float>( 1.0f,  0.0f) / sqrtf(2.0f);
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
	// See equations 3.26 and 3.52
	MatrixXcf P2(n, n);
	P2 = get_star_product(n, P, P);

	cout << "P^2 = " << endl << P2 << endl << endl;

	// See equation 3.53
	cout << "|P^2|^2 = " << endl << get_modulus_squared(n, P2) << endl << endl;

	return 0;
}

