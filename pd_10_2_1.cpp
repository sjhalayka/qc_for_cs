#include <complex>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXcf;
using Eigen::MatrixXcf;

#include <Eigen/Eigenvalues>
using Eigen::ComplexEigenSolver;


int main(void)
{
	// The size of the density matrix depends on the number of orthogonal bases n,
	// isomuch that the number of eigenvalues and eigenvectors are both n as well
	const size_t n = 4;

	// Initialize orthogonal bases
	vector<VectorXcf> orthogonal_bases(n);

	for (size_t i = 0; i < n; i++)
	{
		VectorXcf base(n);

		for (size_t j = 0; j < n; j++)
		{
			if (i == j)
				base(j) = complex<float>(1, 0);
			else
				base(j) = complex<float>(0, 0);
		}

		orthogonal_bases[i] = base;
	}

	// Intialize messages
	vector<VectorXcf> messages(n);

	for (size_t i = 0; i < n; i++)
		messages[i] = orthogonal_bases[i];

	// Get probabilities
	vector<float> probabilities(n, 0.0f);

	// Spice things up with a pseudorandom PDF
	srand(static_cast<unsigned int>(time(0)));

	for (size_t i = 0; i < n; i++)
		probabilities[i] = static_cast<float>(rand() % 256 + 1);

	float grand_total = 0;

	for (size_t i = 0; i < n; i++)
		grand_total += probabilities[i];

	for (size_t i = 0; i < n; i++)
		probabilities[i] /= grand_total;

	// Calculate density matrix
	MatrixXcf density_matrix(n, n);

	density_matrix = probabilities[0] * messages[0] * messages[0].transpose();

	for (size_t i = 1; i < n; i++)
		density_matrix += probabilities[i] * messages[i] * messages[i].transpose();

	// For getting eigenvalues and eigenvectors
	ComplexEigenSolver<MatrixXcf> ces(density_matrix);

	// Calculate von Neumann entropy
	float entropy = 0;

	cout << "Eigenvalues are: " << endl;

	for (int i = 0; i < ces.eigenvalues().rows(); i++)
	{
		const float p = ces.eigenvalues().row(i).col(0)(0).real();
		entropy += p * logf(p);

		cout << p << endl;
	}

	cout << endl;

	cout << "von Neumann entropy is: " << -entropy / logf(2.0f) << endl << endl;

	cout << "Eigenvectors are: " << endl;

	vector<vector<float>> bases;

	for (int i = 0; i < ces.eigenvectors().rows(); i++)
	{
		vector<float> base;

		for (int j = 0; j < ces.eigenvectors().row(i).cols(); j++)
			base.push_back(ces.eigenvectors().row(i).col(j)(0).real());

		bases.push_back(base);
	}

	for (size_t i = 0; i < bases.size(); i++)
	{
		for (size_t j = 0; j < bases[i].size(); j++)
			cout << bases[i][j] << ' ';

		cout << endl;
	}

	return 0;


}




//
//int main(void)
//{
//	// Standard bases
//	VectorXcf basis_zero(2);
//	basis_zero(0) = complex<float>(1, 0);
//	basis_zero(1) = complex<float>(0, 0);
//
//	VectorXcf basis_one(2);
//	basis_one(0) = complex<float>(0, 0);
//	basis_one(1) = complex<float>(1, 0);
//
//	const size_t num_symbols = 2;
//
//	// See example 10.2.2
//	const float p1 = 1.0f / 3.0f;
//	const float p2 = 2.0f / 3.0f;
//
//	const float one_div_sqrt2 = 1.0f / sqrt(2.0f);
//
//	VectorXcf w1 = one_div_sqrt2 * basis_zero + one_div_sqrt2 * basis_one;
//	VectorXcf w2 = basis_zero;
//
//	// Calculate density matrix
//	MatrixXcf density_matrix(num_symbols, num_symbols);
//	density_matrix = p1 * w1 * w1.transpose() +
//		p2 * w2 * w2.transpose();
//
//	ComplexEigenSolver<MatrixXcf> ces(density_matrix);
//
//	// Calculate von Neumann entropy
//	float entropy = 0;
//
//	cout << "Eigenvalues are: " << endl;
//
//	for (int i = 0; i < ces.eigenvalues().rows(); i++)
//	{
//		const float p = ces.eigenvalues().row(i).col(0)(0).real();
//		entropy += p * logf(p);
//
//		cout << p << endl;
//	}
//
//	cout << endl;
//
//	cout << "von Neumann entropy is: " << -entropy / logf(2.0f) << endl << endl;
//
//	cout << "Eigenvectors are: " << endl;
//
//	vector<vector<float>> bases;
//
//	for (int i = 0; i < ces.eigenvectors().rows(); i++)
//	{
//		vector<float> base;
//
//		for (int j = 0; j < ces.eigenvectors().row(i).cols(); j++)
//		{
//			base.push_back(ces.eigenvectors().row(i).col(j)(0).real());
//			//base.push_back(ces.eigenvectors().row(i).col(j)(0).imag()); // always zero?
//		}
//
//		bases.push_back(base);
//	}
//
//	for (size_t i = 0; i < bases.size(); i++)
//	{
//		for (size_t j = 0; j < bases[i].size(); j++)
//		{
//			cout << bases[i][j] << ' ';
//		}
//
//		cout << endl;
//	}
//
//
//
//	return 0;
//
//
//}