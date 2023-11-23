#include <iostream>
#include <complex>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXcf;


int main(void)
{
	const int n = 2;

	VectorXcf v1(n);
	v1(0) = complex<float>(1, -1);
	v1(1) = complex<float>(1, 1);

	VectorXcf v2(n);
	v2(0) = complex<float>(2.0f, 3.0f);	
	v2(1) = complex<float>(-4.0f, -1.0f);

	cout << v1.dot(v2) << endl;

	return 0;
}