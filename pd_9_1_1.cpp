#include <iostream>
#include <string>
using namespace std;

// One could use RSA without padding, which is technically a glorified Caesar cipher

// Instead, we will implement the encryption/decryption wheel in Figure 9.2:
// A -> H
// B ->	I
// C -> J
// D -> K
// E -> L
// F -> M
// G -> N
// H -> O
// I -> P
// J -> Q
// K -> R
// L -> S
// M -> T
// N -> U
// O -> V
// P -> W
// Q -> X
// R -> Y
// S -> Z
// T -> A
// U -> B
// V -> C
// W -> D
// X -> E
// Y -> F
// Z -> G

string ascii_caesar_decrypt(const string& input)
{
	string ret = input;

	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == ' ')
			continue;

		char c = input[i] + 7;

		if (c > 'Z')
			c -= 26;

		ret[i] = c;
	}

	return ret;
}

string ascii_caesar_encrypt(const string& input)
{
	string ret = input;

	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == ' ')
			continue;

		char c = input[i] - 7;

		if (c < 'A')
			c += 26;

		ret[i] = c;
	}

	return ret;
}


int main(void)
{
	// See exercise 9.1.2
	const string ciphertext = "JNTGMNF VHFINMBGZ BL YNG";

	cout << ciphertext << endl;
	cout << ascii_caesar_decrypt(ciphertext) << endl;
	cout << ascii_caesar_encrypt(ascii_caesar_decrypt(ciphertext)) << endl;

	return 0;
}