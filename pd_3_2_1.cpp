#include <iostream>
#include <vector>
#include <complex>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXf;
using Eigen::MatrixXf;


class path
{
public:
	vector<int> vertices;
};

// Modified code, originally from ChatGPT
class graph
{
private:
	vector<vector<int>> adjacencies;

public:
	graph(int v) : adjacencies(v) {}

	void add_half_edge(const int u, const int v)
	{
		adjacencies[v].push_back(u);
	}

	// This function could use some optimization
	bool find_path(const int start, const int end, const int length)
	{
		vector<vector<path>> paths(length);

		// Seed paths
		for (int neighbor : adjacencies[start])
		{
			path p;
			p.vertices.push_back(start);
			p.vertices.push_back(neighbor);
			paths[0].push_back(p);
		}

		// Do next vector of paths
		for (int l = 1; l < length; l++)
		{
			for (size_t i = 0; i < paths[l - 1].size(); i++)
			{
				const int last_vert = paths[l - 1][i].vertices[paths[l - 1][i].vertices.size() - 1];

				for (int neighbor : adjacencies[last_vert])
				{
					path p = paths[l - 1][i];
					p.vertices.push_back(neighbor);
					paths[l].push_back(p);
				}
			}

			// Clean up previous vector of paths
			paths[l - 1].clear();
		}

		// It there at least one match in the final vector of paths?
		for (size_t i = 0; i < paths[paths.size() - 1].size(); i++)
		{
			int final_start = paths[paths.size() - 1][i].vertices[0];
			int final_end = paths[paths.size() - 1][i].vertices[paths[paths.size() - 1][i].vertices.size() - 1];
	
			if (final_start == start && final_end == end)
				return true;
		}

		return false;
	}
};

bool get_path(const int n, const int start, const int end, const int k, const MatrixXf& adjacency_matrix)
{
	graph g(n);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (adjacency_matrix(i, j))
				g.add_half_edge(i, j);

	if (g.find_path(start, end, k))
		return true;

	return false;
}

MatrixXf get_mk(const int n, const int k, const MatrixXf& adjacency_matrix)
{
	if (k == 1)
		return adjacency_matrix;

	MatrixXf ret(n, n);
	ret = adjacency_matrix;

	for (int i = 1; i < k; i++)
		ret = ret * adjacency_matrix;

	return ret;
}


int main(void)
{
	const int n = 3;

	MatrixXf adjacency_matrix(n, n); // aka M^1
	adjacency_matrix <<
		0,           1.0f / 6.0f, 5.0f / 6.0f,
		1.0f / 3.0f, 1.0f / 2.0f, 1.0f / 6.0f,
		2.0f / 3.0f, 1.0f / 3.0f, 0;

	cout << adjacency_matrix << endl << endl;

	VectorXf state_vector(n); // aka X
	state_vector <<
		1.0f / 6.0f, 1.0f / 6.0f, 2.0f / 3.0f;

	cout << state_vector.transpose() << endl << endl;

	const size_t k = 6;

	for (int i = 1; i <= k; i++)
	{
		state_vector = adjacency_matrix * state_vector; // aka Y = MX
		cout << state_vector.transpose() << endl;
	}

	// Get M^k path probability, where k = 1 through 6
	// See equation 3.25 on page 84

	cout << endl;

	for (int i = 1; i <= k; i++)
		cout << "M^" << i << " = " << endl << get_mk(n, i, adjacency_matrix) << endl << endl;

	return 0;
}