#include <iostream>
#include <vector>
#include <bitset>
using namespace std;


template<size_t n>
void knuth2(
	const bitset<n>& bit_sent,
	const bitset<n>& sending_basis,
	const bitset<n>& receiving_basis,
	bitset<n>& bit_received,
	bitset<n>& agreed_bits)
{
	for (size_t i = 0; i < n; i++)
	{
		if (sending_basis[i] == receiving_basis[i])
			bit_received[i] = bit_sent[i];
		else
			bit_received[i] = rand() % 2;
	}
}

int main(void)
{
	const size_t n = 60;

	bitset<n> alice_bit_sent;
	bitset<n> alice_sending_basis;

	for (size_t i = 0; i < n; i++)
	{
		alice_bit_sent[i] = rand() % 2;
		alice_sending_basis[i] = rand() % 2;
	}

	bitset<n> bob_receiving_basis;

	for (size_t i = 0; i < n; i++)
		bob_receiving_basis[i] = rand() % 2;

	bitset<n> bob_bit_received;
	bitset<n> agreed_bits;
	knuth2<n>(alice_bit_sent, alice_sending_basis, bob_receiving_basis, bob_bit_received, agreed_bits);

	cout << "Bit sent:     " << alice_bit_sent << endl;
	cout << "Bit received: " << bob_bit_received << endl;
	cout << "Agreed bits:  " << agreed_bits << endl;
 
	return 0;
}


