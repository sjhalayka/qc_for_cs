#include <iostream>
#include <map>
#include <string>
#include <cmath>
using namespace std;


int main(void)
{
	const string input_string = "Hello world";
	const float length = static_cast<float>(input_string.length());

	float shannon_entropy = 0.0f;

	map<char, size_t> input_map;

	for (size_t i = 0; i < length; i++)
		input_map[input_string[i]]++;

	for (map<char, size_t>::const_iterator ci = input_map.begin(); ci != input_map.end(); ci++)
	{
		const float probability = ci->second / length;

		cout << "Character: \'" << ci->first << "\', Count: " << ci->second << ", Probability: " << probability << endl;

		// Print out a sideways graph, showing probability
		const float max_width = 80.0f;
		const float num_stars = max_width * probability;

		for (float j = 0; j < num_stars; j += 1.0f)
			cout << '*';

		cout << endl << endl;

		shannon_entropy += probability * logf(probability);
	}

	shannon_entropy = -shannon_entropy;

	cout << "shannon_entropy: " << shannon_entropy << endl;

	return 0;
}