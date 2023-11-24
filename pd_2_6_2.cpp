#include <iostream>
#include <complex>
using namespace std;

#include "Eigen/Dense"
using Eigen::MatrixXcf;


int main(void)
{
	const int n = 2;

	MatrixXcf mat1(n, n);

	mat1(0, 0) = complex<float>(1.0f / 2.0f, -1.0f / 2.0f);
	mat1(0, 1) = complex<float>(1.0f / 2.0f,  1.0f / 2.0f);
	mat1(1, 0) = complex<float>(1.0f / 2.0f,  1.0f / 2.0f);
	mat1(1, 1) = complex<float>(1.0f / 2.0f, -1.0f / 2.0f);

	cout << mat1.isUnitary() << endl;

	MatrixXcf mat2 = mat1.transpose().conjugate();

	cout << mat1 * mat2 << endl; // Should be the identity matrix

	return 0;
}