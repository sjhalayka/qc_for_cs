#include <iostream>
#include <vector>
#include <complex>
using namespace std;

#include "Eigen/Dense"
using Eigen::VectorXf;
using Eigen::MatrixXf;


// From ChatGPT
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

	// Depth-first search
	bool DFS(int start, int end, int length, vector<bool>& visited)
	{
		if (start == end && length == 0)
		{
			return true;
		}

		if (length <= 0)
		{
			// Path length exceeded, backtrack
			return false;
		}

		visited[start] = true;

		for (int neighbor : adjacency_list[start])
		{
			if (!visited[neighbor])
			{
				// Recursive DFS call
				if (DFS(neighbor, end, length - 1, visited))
				{
					return true;
				}
			}
		}

		visited[start] = false; // Backtrack
		return false;
	}

	bool find_path(int start, int end, int length)
	{
		vector<bool> visited(vertices, false);
		return DFS(start, end, length, visited);
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

	VectorXf state_vector(n); // aka X
	state_vector <<
		6, 2, 1, 5, 3, 10;

	MatrixXf adjacency_matrix(n, n); // aka M^1
	adjacency_matrix <<
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 1,
		0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0,
		1, 0, 0, 0, 1, 0;

	cout << state_vector << endl;
	cout << adjacency_matrix << endl << endl;

	const size_t k = 6;

	for (size_t i = 0; i < k; i++)
		state_vector = adjacency_matrix * state_vector; // y = mx

	cout << state_vector << endl;


	// Get M^k path connectivity, where k = 1, 2, 3, and 6
	// See equation 3.11 on page 78
	// See exercise 3.1.2 on page 78
	//cout << endl;
	//cout << get_mk(n, 1, adjacency_matrix) << endl << endl;
	//cout << get_mk(n, 2, adjacency_matrix) << endl << endl;
	//cout << get_mk(n, 3, adjacency_matrix) << endl << endl;
	//cout << get_mk(n, 6, adjacency_matrix) << endl << endl;

	return 0;
}