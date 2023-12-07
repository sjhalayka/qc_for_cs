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
	bitset<n>& agreed_bits,
	size_t &agreed_bits_size)
{
	for (size_t i = 0; i < n; i++)
	{
		if (sending_basis[i] == receiving_basis[i])
			bit_received[i] = bit_sent[i];
		else
			bit_received[i] = rand() % 2;
	}

	// Todo: get agreed_bits
	agreed_bits_size = n;

}

int main(void)
{
	// It's not ultra clear in the book whether the author is
	// looking for all 3 bit strings (e.g. alice_bit_sent, etc) or 
	// all 3-bit strings (e.g. "000"), or both, so we do both!

	const size_t n = 3; // number of bits per string

	vector<bitset<n>> bit_sets;

	// Find all permutations of an n-bit string
	for (size_t i = 0; i < (1 << n); i++)
		bit_sets.push_back(bitset<n>(i));

	// Process each bit string
	for (size_t i = 0; i < bit_sets.size(); i++)
	{
		bitset<n> alice_bit_sent;
		bitset<n> alice_sending_basis;

		for (size_t j = 0; j < n; j++)
		{
			alice_bit_sent[j] = bit_sets[i][j];
			alice_sending_basis[j] = rand() % 2;
		}

		bitset<n> bob_receiving_basis;

		for (size_t i = 0; i < n; i++)
			bob_receiving_basis[i] = rand() % 2;

		bitset<n> bob_bit_received;
		bitset<n> agreed_bits;
		size_t agreed_bits_size = 0;
		knuth2<n>(alice_bit_sent, alice_sending_basis, bob_receiving_basis, bob_bit_received, agreed_bits, agreed_bits_size);

		cout << "Bit sent:     " << alice_bit_sent << endl;
		cout << "Bit received: " << bob_bit_received << endl;

		cout << "Agreed bits:  ";

		// In case agreed_bits_size < agreed_bits.size()
		for(size_t j = 0; j < agreed_bits_size; j++)
			cout << agreed_bits[j];
		
		cout << endl << endl;
	}
 
	return 0;
}


