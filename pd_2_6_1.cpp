#include <iostream>
#include <complex>
using namespace std;

#include "Eigen/Dense"
using Eigen::MatrixXcf;


bool is_hermitian(const MatrixXcf& mat)
{
	// See exercise 2.6.2
	MatrixXcf ct1 = mat.conjugate();
	MatrixXcf ct2 = mat.transpose();

	if (ct1 == ct2)
		return true;

	return false;
}


int main(void)
{
	const int n = 2;

	MatrixXcf mat1(n, n);
	mat1(0, 0) = complex<float>(7.0f, 0.0f);
	mat1(1, 0) = complex<float>(6.0f, -5.0f);
	mat1(0, 1) = complex<float>(6.0f, 5.0f);
	mat1(1, 1) = complex<float>(-3.0f, 0.0f);

	cout << is_hermitian(mat1) << endl;

	return 0;
}