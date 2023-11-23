#include <iostream>
#include <complex>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXcf;


int main(void)
{
	const int n = 2;

	VectorXcf v1(n);
	v1(0) = complex<float>(4, -1);
	v1(1) = complex<float>(2, -3);
	
	VectorXcf v2(n);
	v2(0) = complex<float>(1.4f, -1.3f);
	v2(1) = complex<float>(9.2f, -3.0f);

	cout << (v2 - v1).norm() << endl;

	return 0;
}