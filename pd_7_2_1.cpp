#include <regex>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <cctype>
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

bool is_string_binary(const string& s)
{
	if (s == "")
		return false;

	for (size_t i = 0; i < s.size(); i++)
		if (s[i] != '0' && s[i] != '1')
			return false;

	return true;
}

bool is_string_positive_integer(const string& s)
{
	if (s == "")
		return false;

	for (size_t i = 0; i < s.size(); i++)
	{
		const int is_digit = isdigit(s[i]);

		if (0 == is_digit)
			return false;
	}

	return true;
}

bool is_string_alphanumeric(const string& s)
{
	if (s == "")
		return false;

	for (size_t i = 0; i < s.size(); i++)
	{
		const int is_alphanumeric = isalnum(s[i]);

		if (0 == is_alphanumeric)
			return false;
	}

	return true;
}

enum string_type { blank = 0, binary = 4, numeric = 3, alphanumeric = 2, something = 1 };

string_type get_string_type(const string& s)
{
	if (s == "")
		return blank;

	if (is_string_binary(s))
		return binary;

	if (is_string_positive_integer(s))
		return numeric;

	if (is_string_alphanumeric(s))
		return alphanumeric;

	return something;
}

void analyze_instruction_and_params(const string& instruction, vector<vector<string>> params)
{
	cout << instruction << endl;

	for (size_t i = 0; i < params.size(); i++)
	{
		for (size_t j = 0; j < params[i].size(); j++)
			cout << params[i][j] << ' ';

		cout << endl;
	}

	size_t min_params = static_cast<size_t>(-1); // Casting turns it into the biggest integer value
	size_t max_params = 0;

	for (size_t i = 0; i < params.size(); i++)
	{
		if (params[i].size() > max_params)
			max_params = params[i].size();

		if (params[i].size() < min_params)
			min_params = params[i].size();
	}

	for (size_t i = 0; i < params.size(); i++)
	{
		size_t num_blanks_to_add = max_params - params[i].size();

		for (size_t j = 0; j < num_blanks_to_add; j++)
			params[i].push_back("");
	}

	//	cout << min_params << " " << max_params << endl;

	vector<string_type> string_types(max_params, blank);

	for (size_t i = 0; i < params.size(); i++)
	{
		for (size_t j = 0; j < params[i].size(); j++)
		{
			if (params[i][j] == "")
				continue;

			//cout << params[i][j] << endl;

			string_type st = get_string_type(params[i][j]);

			if (string_types[j] == blank || st <  string_types[j])
				string_types[j] = st;
		}
	}

	for (size_t i = 0; i < string_types.size(); i++)
	{
		if (string_types[i] == blank)
			cout << "blank";
		else if (string_types[i] == binary)
			cout << "binary";
		else if (string_types[i] == numeric)
			cout << "numeric";
		else if (string_types[i] == alphanumeric)
			cout << "alphanumeric";
		else if (string_types[i] == something)
			cout << "something";
	
		if (i < min_params)
			cout << " (not optional)" << endl;
		else
			cout << " (optional)" << endl;
	}

	cout << endl;

}


int main(void)
{
	map<string, vector<vector<string>>> instructions_and_params;

	vector<string> lines = {
		"var B=[00001111] // before invoking quantum assembler",
		"INITIALIZE R1",
		"INITIALIZE R1 B",
		"INITIALIZE R 2",
		"U TENSOR H I2",
		"APPLY U R",
		"SELECT S1 R 0 1",
		"MEASURE S1 RES",
		"APPLY CNOT R",
		"MEASURE R RES",
		"IF RES==[10] THEN APPLY CNOT R ELSE APPLY H R" };

	for (size_t i = 0; i < lines.size(); i++)
	{
		// Chop off the comments
		vector<string> vs = std_strtok(lines[i], "[//]\\s*");

		if (vs.size() == 0)
			continue;

		// Only keep the first token
		vs.resize(1);

		// Tokenize the rest by whitespace
		vs = std_strtok(vs[0], "[ \t]\\s*");

		// Separate the data
		string instruction = vs[0];
		vector<string> params = vs;
		params.erase(params.begin(), params.begin() + 1);

		instructions_and_params[instruction].push_back(params);
	}

	// Classify type by contents
	for (const auto pair : instructions_and_params)
		analyze_instruction_and_params(pair.first, pair.second);

	return 0;
}
