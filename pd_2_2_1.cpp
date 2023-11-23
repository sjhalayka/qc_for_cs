#include <iostream>
#include <complex>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXcf;
using Eigen::MatrixXcf;


int main(void)
{
	const int m = 2;
	const int n = 2;

	MatrixXcf mat1(m, n);
	mat1(0, 0) = complex<float>(3.0f, -10.0f);
	mat1(1, 0) = complex<float>(2.0f, 2.4f);
	mat1(0, 1) = complex<float>(-1.0f, -3.0f);
	mat1(1, 1) = mat1(1, 0) + mat1(0, 1);

	MatrixXcf mat2(m, n);
	mat2(0, 0) = complex<float>(7.3f, 8.0f);
	mat2(1, 0) = complex<float>(2.0f, 0.75f);
	mat2(0, 1) = complex<float>(4.0f, 0.0f);
	mat2(1, 1) = mat2(1, 0) + mat2(0, 1);

	const MatrixXcf sum = mat1 + mat2;
	const MatrixXcf inv = -mat1;
	const float s = 1.1f;
	const MatrixXcf prod = mat1 * s;

	cout << mat1 << endl << endl << mat2 << endl << endl;
	cout << sum << endl << endl << inv << endl << endl << prod << endl;

	return 0;
}