#include <iostream>
#include <vector>
#include <bitset>
using namespace std;


int main(void)
{
	const int n = 6;

	bitset<n> message_bits("101001");
	bitset<n> one_time_pad_bits("010101");

	cout << "Message bits:      " << message_bits << endl;
	cout << "One time pad bits: " << one_time_pad_bits << endl;

	bitset<n> encrypted_bits = message_bits ^ one_time_pad_bits;
	bitset<n> decrypted_bits = encrypted_bits ^ one_time_pad_bits;

	cout << "Encrypted bits:    " << encrypted_bits << endl << "Decrypted bits:    " << decrypted_bits << endl;

	return 0;
}