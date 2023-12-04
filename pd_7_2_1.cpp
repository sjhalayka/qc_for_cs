#include <regex>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
using namespace std;


vector<string> std_strtok(const string& s, const string& regex_s)
{
	vector<string> tokens;

	regex r(regex_s);

	sregex_token_iterator iter(s.begin(), s.end(), r, -1);
	sregex_token_iterator end;

	while (iter != end)
	{
		if (*iter != "")
			tokens.push_back(*iter);

		iter++;
	}

	return tokens;
}

int main(void)
{
	map<string, vector<vector<string>>> instructions_and_params;

	vector<string> lines = {"var B=[00001111] // before invoking quantum assembler",
							"INITIALIZE R1",
							"INITIALIZE R1 B",
							"INITIALIZE R 2",
							"U TENSOR H I2",
							"APPLY U R",
							"SELECT S1 R 0 1",
							"MEASURE S1 RES",
							"APPLY CNOT R",
							"MEASURE R RES",
							"IF (RES==[10]) THEN APPLY CNOT R ELSE APPLY H R" };

	for(size_t i = 0; i < lines.size(); i++)
	{
		string line = lines[i];

		// Chop off the comments
		vector<string> vs = std_strtok(line, "[//]\\s*");

		if (vs.size() == 0)
			continue;

		vs.resize(1);

		// Tokenize the rest by whitespace
		vs = std_strtok(vs[0], "[ \t]\\s*");

		// Separate the data
		string instruction = vs[0];
		vector<string> params = vs;
		params.erase(params.begin(), params.begin() + 1);

		instructions_and_params[instruction].push_back(params);
	}

	for (const auto pair : instructions_and_params)
	{
		cout << pair.first << endl;

		for (size_t i = 0; i < pair.second.size(); i++)
		{
			for (size_t j = 0; j < pair.second[i].size(); j++)
			{
				cout << pair.second[i][j] << ' ';
			}

			cout << endl;
		}

		cout << endl;
	}

	// Classify type by contents

	return 0;
}
