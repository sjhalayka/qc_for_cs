#include <iostream>
#include <complex>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXcf;


int main(void)
{
	const int n = 4;

	VectorXcf v1(n);
	v1(0) = complex<float>(1, 0.5f);
	v1(1) = complex<float>(2, 4);
	v1(2) = complex<float>(-1, 0.75f);
	v1(3) = complex<float>(3.1f, -7);

	VectorXcf v2(n);
	v2(0) = complex<float>(11, 0.15f);
	v2(1) = complex<float>(12, 14);
	v2(2) = complex<float>(-11, 1.75f);
	v2(3) = complex<float>(13.1f, -17);

	const VectorXcf sum = v1 + v2;
	const VectorXcf inv = -v1;
	const float s = 1.1f;
	const VectorXcf prod = v1 * s;

	cout << v1 << endl << v2 << endl << endl;
	cout << sum << endl << inv << endl << prod << endl;

	return 0;
}