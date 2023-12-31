#include <complex>
#include <iostream>
#include <vector>
#include <random>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXcf;
using Eigen::MatrixXcf;

#include <Eigen/Eigenvalues>
using Eigen::ComplexEigenSolver;


int main(void)
{
	// The width and height of the density matrix depends on the 
	// number of orthogonal bases n. The number of eigenvalues and
	// eigenvectors are both n as well.
	// 
	// Here we will use a distinct base for each message
	// simply because we can do so

	const size_t n = 4;

	// Initialize orthogonal bases
	vector<VectorXcf> orthogonal_bases(n);

	for (size_t i = 0; i < n; i++)
	{
		VectorXcf base(n);

		// Note:
		// Keeping things real and orthogonal means that the
		// density matrix is zero outside of the diagonal,
		// and that the entries for the diagonal are the
		// eigenvalues
		//
		// If it's not orthogonal, then the density matrix
		// is generally non-zero outside of the diagonal, 
		// and the entries for the diagonal are not the eigenvalues
		//
		// If it's imaginary, then the entropy becomes complex

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
	// Spice things up with a pseudorandom PDF
	// Use Mersenne Twister for the pseudorandom number generator
	vector<float> probabilities(n);

	mt19937 mt;
	mt.seed(static_cast<unsigned int>(time(0)));

	float grand_total = 0;

	for (size_t i = 0; i < n; i++)
	{
		probabilities[i] = static_cast<float>(mt() % 256 + 1); // Larger than zero
		grand_total += probabilities[i];
	}

	for (size_t i = 0; i < n; i++)
		probabilities[i] /= grand_total;


	// Calculate density matrix
	// See equation 10.14
	MatrixXcf density_matrix(n, n);
	density_matrix.setZero(); // IMPORTANT

	for (size_t i = 0; i < n; i++)
		density_matrix += probabilities[i] * messages[i] * messages[i].transpose();

	cout << "Density matrix =" << endl << density_matrix << endl << endl;


	// For getting eigenvalues and eigenvectors
	const ComplexEigenSolver<MatrixXcf> ces(density_matrix);


	// Calculate von Neumann entropy
	complex<float> entropy = 0;

	cout << "Eigenvalues are: " << endl;

	for (int i = 0; i < ces.eigenvalues().rows(); i++)
	{
		const complex<float> p = ces.eigenvalues().row(i).col(0)(0);
		
		cout << p << endl;
		
		entropy += p * log(1.0f/p);
	}

	cout << endl;

	cout << "von Neumann entropy is: " << entropy / logf(2.0f) << endl << endl;


	cout << "Eigenvectors are: " << endl;

	vector<vector<complex<float>>> bases;

	for (int i = 0; i < ces.eigenvectors().rows(); i++)
	{
		vector<complex<float>> base;

		for (int j = 0; j < ces.eigenvectors().row(i).cols(); j++)
			base.push_back(ces.eigenvectors().row(i).col(j)(0));

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
