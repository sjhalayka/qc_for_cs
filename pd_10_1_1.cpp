#include <iostream>
#include <map>
#include <string>
#include <cmath>
using namespace std;


int main(void)
{
	const string input_string = "Hello world";
	const size_t length = input_string.length();

	float total_probability = 0.0f;
	float binary_shannon_entropy = 0.0f;

	map<char, size_t> input_map;

	for (size_t i = 0; i < length; i++)
		input_map[input_string[i]]++;

	for (map<char, size_t>::const_iterator ci = input_map.begin(); ci != input_map.end(); ci++)
	{
		const float probability = ci->second / static_cast<float>(length);
		total_probability += probability;

		// See Definition 10.1.1, where we use the binary Shannon entropy
		binary_shannon_entropy += probability * (logf(1.0f / probability) / logf(2.0f));

		// Print out the data
		cout << "Character: \'" << ci->first << "\', Count: " << ci->second << ", Probability: " << probability << endl;

		// Print out a sideways graph, signifying probability
		const size_t max_width = 80;
		const size_t num_stars = static_cast<size_t>(max_width * probability);

		for (size_t j = 0; j < num_stars; j++)
			cout << '*';

		cout << endl << endl;
	}

	cout << "Total probability:      " << total_probability << endl; // Should always be 1.0f
	cout << "Binary Shannon entropy: " << binary_shannon_entropy << endl;

	return 0;
}