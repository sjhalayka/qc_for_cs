#include <iostream>
#include <string>
using namespace std;


string ascii_caesar_encrypt(const string& input)
{
	string ret = input;

	for (size_t i = 0; i < input.size(); i++)
	{
		char c = input[i] + 7;

		if (c != ' ' && c > 'Z')
			c -= 26;

		ret[i] = c;
	}

	return ret;
}

string ascii_caesar_decrypt(const string& input)
{
	string ret = input;

	for (size_t i = 0; i < input.size(); i++)
	{
		char c = input[i] - 7;

		if (c != ' ' && c < 'A')
			c += 26;

		ret[i] = c;
	}

	return ret;
}


int main(void)
{
	// One could use RSA without padding, which is technically a glorified Caesar cipher
	// Instead, we will implement the encryption/decryption wheel in Figure 9.2

	string plaintext = "HELLO WORLD";
	string ciphertext = ascii_caesar_encrypt(plaintext);
	string decrypted_text = ascii_caesar_decrypt(ciphertext);

	cout << plaintext << endl << ciphertext << endl << decrypted_text << endl;

	return 0;
}