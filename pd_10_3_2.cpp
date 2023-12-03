#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <complex>
using namespace std;


// Modified from:
// https://gist.github.com/pwxcoo/72d7d3c5c3698371c21e486722f9b34b
//
// also see:
// Mastering Algorithms with C by Kyle Loudon



// For use of complex<float> with map
bool operator<(const complex<float>& left, const complex<float>& right)
{
	if (right.real() > left.real())
		return true;
	else if (right.real() < left.real())
		return false;

	if (right.imag() > left.imag())
		return true;
	else if (right.imag() < left.imag())
		return false;

	return false;
}

template <typename T>
class huffman_codec
{
public:
	template <typename T>
	class Node
	{
	public:
		Node(void)
		{
			ch = 0;
			freq = 0;
			left = right = nullptr;
		}

		T ch;
		int freq;
		Node<T>* left, * right;
	};

private:
	vector<Node<T>*> nodes_to_clean_up;
	map<T, string> huffman_codes;

	size_t map_bit_count;
	vector<T> text;

public:
	huffman_codec(const vector<T>& plaintext)
	{
		set_plaintext(plaintext);
	}

	huffman_codec(void)
	{
		map_bit_count = 0;
		text.clear();
	}

	~huffman_codec(void)
	{ 
		clean_up(); 
	}

	void set_plaintext(const vector<T>& plaintext)
	{
		map_bit_count = 0;

		if (plaintext.size() == 0)
			return;

		text = plaintext;

		clean_up();

		init_huffman_codes(text);

		for (const auto pair : huffman_codes)
			map_bit_count += sizeof(T) * 8 + pair.second.size(); // m bits per key + n bits per element
	}

	size_t get_map_bit_count(void)
	{
		return map_bit_count;
	}

	void print_huffman_codes(void)
	{
		cout << "Huffman codes:" << endl;

		for (auto pair : huffman_codes)
			cout << pair.first << " " << pair.second << endl;

		cout << endl;
	}

	void get_huffman_codes(map<T, string> &huffman_codes_output)
	{
		huffman_codes_output = huffman_codes;
	}
	
	bool get_encoded_string(string &encoded_string)
	{
		encoded_string = "";

		for (const T c : text)
			encoded_string += huffman_codes[c];

		return true;
	}

	bool get_decoded_string(const string& encoded_string, vector<T>& decoded_string)
	{
		decoded_string.clear();

		if (huffman_codes.size() == 0 || encoded_string == "")
			return false;

		if (huffman_codes.size() == 1)
		{
			const T c = huffman_codes.begin()->first; // should be '0'

			for (size_t i = 0; i < encoded_string.size(); i++)
				decoded_string.push_back(c);

			return true;
		}

		// Get the minimum and maximum token size
		size_t min_bits = static_cast<size_t>(-1); // Casting the number -1 transforms it into the largest possible value that can be held by a size_t
		size_t max_bits = 0;

		for (const auto pair : huffman_codes)
		{
			if (pair.second.size() < min_bits)
				min_bits = pair.second.size();

			if (pair.second.size() > max_bits)
				max_bits = pair.second.size();
		}

		const size_t encoded_len = encoded_string.length();

		// Use a sliding window of variable length
		size_t begin_index = 0; // Start at the very beginning of the string
		size_t len = min_bits; // Don't waste time by starting at len = 1 if it's not necessary

		// While stuff to parse 
		while (is_valid_window(encoded_len, begin_index, len))
		{
			if (len > max_bits)
			{
				// No match was found up to here, which is exceptionally weird
				return false;
			}

			// Match token with map element
			const string token = encoded_string.substr(begin_index, len);

			bool found_token = false;

			for (const auto pair : huffman_codes)
			{
				if (pair.second == token)
				{
					decoded_string.push_back(pair.first);
					found_token = true;
					break;
				}
			}

			if (found_token)
			{
				// Slide window by token size number of steps,
				// then reset window length to the minumum
				begin_index += token.size();
				len = min_bits;
			}
			else
			{
				// Expand window length by 1
				len++;
			}
		}

		return true;
	}

private:
	// traverse the Huffman Tree and store Huffman Codes
	// in a map.
	void encode(Node<T>* root, const string str, map<T, string>& huffman_codes)
	{
		if (root == nullptr)
			return;

		// found a leaf node
		if (!root->left && !root->right)
			huffman_codes[root->ch] = str;

		encode(root->left, str + "0", huffman_codes);
		encode(root->right, str + "1", huffman_codes);
	}
	
	// Function to allocate a new tree node
	Node<T>* getNode(const T ch, const int freq, Node<T>* left, Node<T>* right)
	{
		Node<T>* node = new Node<T>();
		nodes_to_clean_up.push_back(node);

		node->ch = ch;
		node->freq = freq;
		node->left = left;
		node->right = right;

		return node;
	}

	// Comparison object to be used to order the heap
	struct node_comp
	{
		bool operator()(const Node<T>* l, const Node<T>* r)
		{
			// highest priority item has lowest frequency
			return l->freq > r->freq;
		}
	};

	void clean_up(void)
	{
		if (nodes_to_clean_up.size() == 0)
			return;

		//cout << "Cleaning up " << nodes_to_clean_up.size() << " nodes." << endl;

		for (size_t i = 0; i < nodes_to_clean_up.size(); i++)
			delete nodes_to_clean_up[i];

		nodes_to_clean_up.clear();
	}

	bool is_valid_window(const size_t string_length, const size_t begin_window_index, const size_t window_length) const
	{
		if (begin_window_index >= string_length)
			return false;

		if ((begin_window_index + window_length - 1) >= string_length)
			return false;

		return true;
	}

	void init_huffman_codes(const vector<T>& input)
	{
		huffman_codes.clear();

		if (input.size() == 0)
			return;

		Node<T>* root = nullptr;

		// count frequency of appearance of each character
		// and store it in a map
		map<T, int> freq;

		for (T ch : input)
			freq[ch]++;

		if (freq.size() == 1)
		{
			root = getNode(freq.begin()->first, freq.begin()->second, nullptr, nullptr);
			huffman_codes[root->ch] = "0";

			return;
		}

		// Create a priority queue to store live nodes of
		// Huffman tree;
		priority_queue<Node<T>*, vector<Node<T>*>, node_comp> pq;

		// Create a leaf node for each character and add it
		// to the priority queue.
		for (auto pair : freq)
			pq.push(getNode(pair.first, pair.second, nullptr, nullptr));

		// do till there is more than one node in the queue
		while (pq.size() != 1)
		{
			// Remove the two nodes of highest priority
			// (lowest frequency) from the queue
			Node<T>* left = pq.top();
			pq.pop();

			Node<T>* right = pq.top();
			pq.pop();

			// Create a new internal node with these two nodes
			// as children and with frequency equal to the sum
			// of the two nodes' frequencies. Add the new node
			// to the priority queue.
			int sum = left->freq + right->freq;

			pq.push(getNode('\0', sum, left, right));
		}

		// root stores pointer to root of Huffman Tree
		root = pq.top();

		// traverse the Huffman Tree and store Huffman Codes
		// in a map. Also prints them

		encode(root, "", huffman_codes);
	}
};


int main(void)
{
	const complex<float> u = 1.0f;
	const complex<float> a = u / sqrtf(2.0f);
	const complex<float> b = complex<float>(-1.0f,  1.0f) / sqrtf(6.0f);
	const complex<float> c = complex<float>(-1.0f, -1.0f) / sqrtf(6.0f);
	const complex<float> d = complex<float>( 1.0f, -1.0f) / sqrtf(6.0f);

	// Vectors with lower entropy produce higher compression rates
	vector<complex<float>> plaintext = { a, b, c, d, a, u, u, u };


	huffman_codec<complex<float>> h;

	h.set_plaintext(plaintext);
	h.print_huffman_codes();


	cout << "Original vector was: ";

	for (size_t i = 0; i < plaintext.size(); i++)
		cout << plaintext[i] << ' ';

	cout << endl;


	string encoded_string = "";
	h.get_encoded_string(encoded_string);
	cout << "Encoded string is:   " << encoded_string << endl;


	vector<complex<float>> decoded_string;
	h.get_decoded_string(encoded_string, decoded_string);
	cout << "Decoded vector is:   ";

	for (size_t i = 0; i < decoded_string.size(); i++)
		cout << decoded_string[i] << ' ';

	cout << endl;


	// The number of map bits becomes negligible for large encoded string length
	size_t num_map_bits = h.get_map_bit_count();

	size_t num_encoded_bits = encoded_string.size() + num_map_bits;
	size_t num_decoded_bits = decoded_string.size() * sizeof(complex<float>)*8;
	float scale = static_cast<float>(num_encoded_bits) / static_cast<float>(num_decoded_bits);

	// Scale is less than 1.0 if compression occurs
	cout << "Scale: " << scale << endl;

	return 0;
}