#include <iostream>
#include <vector>
using namespace std;


int main(void)
{
	vector<bool> message_bits = { 1, 0, 1, 0, 0, 1 };
	vector<bool> one_time_pad_bits = { 0, 1, 0, 1, 0, 1 };

	vector<bool> encrypted_bits;

	for (size_t i = 0; i < message_bits.size(); i++)
		encrypted_bits.push_back(message_bits[i] ^ one_time_pad_bits[i]);

	vector<bool> decrypted_bits;

	for (size_t i = 0; i < encrypted_bits.size(); i++)
		decrypted_bits.push_back(encrypted_bits[i] ^ one_time_pad_bits[i]);

	for (size_t i = 0; i < decrypted_bits.size(); i++)
		cout << static_cast<unsigned short int>(decrypted_bits[i]);

	return 0;
}