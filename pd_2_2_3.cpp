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

	VectorXcf v1(n);
	v1(0) = complex<float>(7.3f, 8.0f);
	v1(1) = complex<float>(2.0f, 0.75f);

	cout << mat1*v1 << endl;

	return 0;
}