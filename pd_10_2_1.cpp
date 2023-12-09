#include <complex>
#include <iostream>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXcf;
using Eigen::MatrixXcf;

// https://eigen.tuxfamily.org/dox/classEigen_1_1ComplexEigenSolver.html
#include <Eigen/Eigenvalues>
using Eigen::ComplexEigenSolver;



// https://physics.stackexchange.com/questions/377089/is-there-a-clear-and-intuitive-meaning-to-the-eigenvectors-and-eigenvalues-of-a

int main(void)
{
	VectorXcf basis_zero(2);
	basis_zero(0) = complex<float>(1, 0);
	basis_zero(1) = complex<float>(0, 0);

	VectorXcf basis_one(2);
	basis_one(0) = complex<float>(0, 0);
	basis_one(1) = complex<float>(1, 0);

	const float p1 = 1.0f / 3.0f;
	const float p2 = 2.0f / 3.0f;

	const float one_div_sqrt2 = 1.0f / sqrt(2.0f);

	// See example 10.2.2
	VectorXcf w1 = one_div_sqrt2 * basis_zero + one_div_sqrt2 * basis_one;
	VectorXcf w2 = basis_zero;

	MatrixXcf D(2, 2);
	D = p1 * w1 * w1.transpose() + p2 * w2 * w2.transpose();

	//cout << D << endl << endl;

	ComplexEigenSolver<MatrixXcf> ces(D);

	float entropy = 0;

	for (int i = 0; i < ces.eigenvalues().rows(); i++)
	{
		const float p = ces.eigenvalues().row(i).col(0)(0).real();
		entropy += p * logf(p);
	}

	cout << "von Neumann entropy is: " << -entropy / logf(2.0f) << endl;

	cout << "Eigenvectors are: " << endl;

	for (int i = 0; i < ces.eigenvectors().rows(); i++)
		cout << ces.eigenvectors().row(i) << endl;

	return 0;

	
}