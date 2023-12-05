#include <iostream>
#include <vector>
#include <bitset>
using namespace std;


template<size_t n>
float knuth(
	const bitset<n> &bit_sent, 
	const bitset<n> &sending_basis, 
	const bitset<n> &receiving_basis,
	bitset<n>& bit_received)

{
	for (size_t i = 0; i < n; i++)
	{
		if (sending_basis[i] == receiving_basis[i])
			bit_received[i] = bit_sent[i];
		else
			bit_received[i] = rand() % 2;
	}

	float correctness_percentage = 0.0f;

	for (size_t i = 0; i < n; i++)
		if (bit_sent[i] == bit_received[i])
			correctness_percentage += 1.0;

	correctness_percentage /= n;

	return correctness_percentage * 100.0f;
}

int main(void)
{
	const int n = 60;

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
	float correctness = knuth<n>(alice_bit_sent, alice_sending_basis, bob_receiving_basis, bob_bit_received);

	cout << "Bit sent:     " << alice_bit_sent << endl;
	cout << "Bit received: " << bob_bit_received << endl;

	cout << correctness << "% correct" << endl;

	return 0;
}