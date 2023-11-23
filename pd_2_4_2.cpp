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

	cout << v1.norm() << endl << endl;
	cout << sqrt(v1.dot(v1)) << endl;

	return 0;
}