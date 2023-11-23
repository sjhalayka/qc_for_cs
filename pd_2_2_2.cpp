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

	if (mat1.cols() != mat2.rows())
	{
		cout << "Invalid input" << endl;
		return -1;
	}

	cout << mat1 * mat2 << endl << endl;
	cout << mat2 * mat1 << endl;

	return 0;
}