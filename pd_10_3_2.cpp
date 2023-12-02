#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;


// Modified from:
// https://gist.github.com/pwxcoo/72d7d3c5c3698371c21e486722f9b34b
//
// also see:
// Mastering Algorithms with C by Kyle Loudon

template <typename T>
class huffman_codec
{
	class Node
	{
	public:
		T ch;
		int freq;
		Node* left, * right;
	};

private:
	vector<Node*> nodes_to_clean_up;
	unordered_map<T, string> huffman_codes;

	size_t map_bit_count;
	basic_string<T> text;

public:
	huffman_codec(const string &plaintext)
	{
		set_plaintext(plaintext);
	};

	huffman_codec(void)
	{
		map_bit_count = 0;
		text.clear();
	}

	~huffman_codec(void)
	{ 
		clean_up(); 
	};

	void set_plaintext(const basic_string<T>& plaintext)
	{
		map_bit_count = 0;

		if (plaintext.size() == 0)
			return;

		text = plaintext;

		clean_up();

		init_huffman_codes(text);

		for (const auto pair : huffman_codes)
			map_bit_count += sizeof(T) * 8 + pair.second.size(); // 8 bits per key + n bits per element
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

	void get_huffman_codes(unordered_map<T, string> &huffman_codes_output)
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

	bool get_decoded_string(const string& encoded_string, basic_string<T>& decoded_string)
	{
		decoded_string.clear();

		if (huffman_codes.size() == 0 || encoded_string == "")
			return false;

		if (huffman_codes.size() == 1)
		{
			const T c = huffman_codes.begin()->first; // should be '0'

			for (size_t i = 0; i < encoded_string.size(); i++)
				decoded_string += c;

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
					decoded_string += pair.first;
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
	void encode(Node* root, const string str, unordered_map<T, string>& huffman_codes)
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
	Node* getNode(const T ch, const int freq, Node* left, Node* right)
	{
		Node* node = new Node();
		nodes_to_clean_up.push_back(node);

		node->ch = ch;
		node->freq = freq;
		node->left = left;
		node->right = right;

		return node;
	}

	// Comparison object to be used to order the heap
	struct comp
	{
		bool operator()(Node* l, Node* r)
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

	bool is_valid_window(const size_t string_length, const size_t begin_window_index, const size_t window_length)
	{
		if (begin_window_index >= string_length)
			return false;

		if ((begin_window_index + window_length - 1) >= string_length)
			return false;

		return true;
	}

	void init_huffman_codes(const basic_string<T>& input)
	{
		huffman_codes.clear();

		if (input.size() == 0)
			return;

		Node* root = nullptr;

		// count frequency of appearance of each character
		// and store it in a map
		unordered_map<T, int> freq;

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
		priority_queue<Node*, vector<Node*>, comp> pq;

		// Create a leaf node for each character and add it
		// to the priority queue.
		for (auto pair : freq)
			pq.push(getNode(pair.first, pair.second, nullptr, nullptr));

		// do till there is more than one node in the queue
		while (pq.size() != 1)
		{
			// Remove the two nodes of highest priority
			// (lowest frequency) from the queue
			Node* left = pq.top();
			pq.pop();

			Node* right = pq.top();
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
	// Strings with lower entropy produce higher compression rates
	//string plaintext = "AAAAAAAAAAAAAAAAAAAAAAAAAA";
	//string plaintext = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	//string plaintext;

	//for (size_t i = 0; i < 10000000; i++)
	//	plaintext += rand() % 26 + 'A';


	// Use any type with the basic_string, not just chars
	basic_string<float> plaintext = { 1.0f, 0.5f, 1.0f, -0.25f };

	huffman_codec<float> h;
	h.set_plaintext(plaintext);
	h.print_huffman_codes();

	cout << "Original string was: ";

	for (size_t i = 0; i < plaintext.size(); i++)
		cout << plaintext[i] << ' ';

	cout << endl;


	string encoded_string = "";
	h.get_encoded_string(encoded_string);
	cout << "Encoded string is:   " << encoded_string << endl;


	basic_string<float> decoded_string;
	h.get_decoded_string(encoded_string, decoded_string);
	cout << "Decoded string is:   ";

	for (size_t i = 0; i < decoded_string.size(); i++)
		cout << decoded_string[i] << ' ';

	cout << endl;


	// The number of map bits becomes negligible for large encoded string length
	size_t num_map_bits = h.get_map_bit_count();

	size_t num_encoded_bits = encoded_string.size() + num_map_bits;
	size_t num_decoded_bits = decoded_string.size() * sizeof(float)*8;
	float scale = static_cast<float>(num_encoded_bits) / static_cast<float>(num_decoded_bits);

	// Scale is less than 1.0 if compression occurs
	cout << "Scale: " << scale << endl;

	return 0;
}