#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
using namespace std;


int main(void)
{
	const float pdf_0 = 2.0f / 3.0f;
	const float pdf_1 = 1.0f / 3.0f;

	vector<string> typical_bit_strings;

	const int n = 8;
	const int typical_n_ones = static_cast<int>(floorf(static_cast<float>(n)*pdf_1));

	cout << "Typical count of ones = " << typical_n_ones << endl;

	for (int i = 0; i < (1 << n); i++) 
	{
		bitset<n> b(i);

		int one_count = 0;

		for (size_t j = 0; j < n; j++)
			if (b[j] == 1)
				one_count++;

		if (one_count == typical_n_ones)
			typical_bit_strings.push_back(b.to_string());
	}

	cout << typical_bit_strings.size() << " of " << pow(2, n) << " strings are typical" << endl;

	return 0;
}