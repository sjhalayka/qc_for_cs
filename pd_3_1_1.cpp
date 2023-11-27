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
	int vertices;
	vector<vector<int>> adjacency_list;

public:
	graph(int v) : vertices(v), adjacency_list(v) {}

	void add_half_edge(const int u, const int v)
	{
		adjacency_list[v].push_back(u);
	}

	bool find_path(int start, int end, int length)
	{
		vector<vector<path>> paths(length);

		// Seed paths
		for (int neighbor : adjacency_list[start])
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

				for (int neighbor : adjacency_list[last_vert])
				{
					path p = paths[l - 1][i];
					p.vertices.push_back(neighbor);
					paths[l].push_back(p);
				}
			}

			// Clean up previous vector of paths
			paths[l - 1].clear();
		}

		// It there at least one match in the last vector of paths?
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

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			// See footnote 1 at the bottom of page 76,
			// where it says to trace from j to i, instead from i to j
			ret(i, j) = get_path(n, j, i, k, adjacency_matrix);
		}
	}

	return ret;
}


int main(void)
{
	const int n = 6;

	MatrixXf adjacency_matrix(n, n); // aka M^1
	adjacency_matrix <<
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 1,
		0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0,
		1, 0, 0, 0, 1, 0;

	cout << adjacency_matrix << endl << endl;

	VectorXf state_vector(n); // aka X
	state_vector <<
		//6, 2, 1, 5, 3, 10;
		0, 0, 27, 0, 0, 0;

	cout << state_vector.transpose() << endl << endl;

	// Show the 3-cycle that the marbles follow when all marbles start at vertex 2
	const size_t k = 6;

	for (size_t i = 1; i <= k; i++)
	{
		state_vector = adjacency_matrix * state_vector; // aka Y = MX
		cout << state_vector.transpose() << endl;
	}

	// Get M^k path connectivity, where k = 1, 2, 3, and 6
	// See equation 3.11 on page 78
	// See exercise 3.1.2 on page 78
	cout << endl;
	cout << get_mk(n, 1, adjacency_matrix) << endl << endl;
	cout << get_mk(n, 2, adjacency_matrix) << endl << endl;
	cout << get_mk(n, 3, adjacency_matrix) << endl << endl;
	cout << get_mk(n, 6, adjacency_matrix) << endl << endl;

	return 0;
}