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

struct Node
{
	char ch;
	int freq;
	Node* left, * right;
};

vector<Node*> nodes_to_clean_up;

// Function to allocate a new tree node
Node* getNode(const char ch, const int freq, Node* left, Node* right)
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

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encode(Node* root, const string str, unordered_map<char, string>& huffman_codes)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right)
		huffman_codes[root->ch] = str;

	encode(root->left, str + "0", huffman_codes);
	encode(root->right, str + "1", huffman_codes);
}

void clean_up(void)
{
	cout << "Cleaning up " << nodes_to_clean_up.size() << " nodes." << endl;

	for (size_t i = 0; i < nodes_to_clean_up.size(); i++)
		delete nodes_to_clean_up[i];
}

bool is_valid_window(const size_t string_length, const size_t begin_window_index, const size_t window_length)
{
	if (begin_window_index >= string_length)
		return false;

	if ((begin_window_index + window_length - 1) >= string_length)
		return false;

	return true;
}

void decode(const string &encoded_string, string& decoded_string, const unordered_map<char, string>& huffman_codes)
{
	decoded_string = "";

	if(huffman_codes.size() == 0 || encoded_string == "")
		return;

	if (huffman_codes.size() == 1)
	{
		const char c = huffman_codes.begin()->first; // should be '0'

		for (size_t i = 0; i < encoded_string.size(); i++)
			decoded_string += c;

		return;
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
		//if (len > max_bits)
		//{
		//	// Something weird happened
		//}

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
}

void get_codes(const string& input, unordered_map<char, string>& huffman_codes)
{
	huffman_codes.clear();

	if (input.size() == 0)
		return;

	Node *root = nullptr;

	// count frequency of appearance of each character
	// and store it in a map
	unordered_map<char, int> freq;

	for (char ch : input)
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


int main(void)
{
	// Strings with lower entropy produce higher compression rates
	//string plaintext = "AAAAAAAAAAAAAAAAAAAAAAAAAA";
	string plaintext = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	//string plaintext;

	//for (size_t i = 0; i < 100; i++)
	//	plaintext += rand() % 26 + 'A';


	unordered_map<char, string> huffman_codes;
	get_codes(plaintext, huffman_codes);

	cout << "Huffman codes:" << endl;

	for (auto pair : huffman_codes)
		cout << pair.first << " " << pair.second << endl;

	cout << endl;


	cout << "Original string was: " << plaintext << endl;


	string encoded_string = "";

	for (char c : plaintext)
		encoded_string += huffman_codes[c];

	cout << "Encoded string is:   " << encoded_string << endl;


	string decoded_string;
	decode(encoded_string, decoded_string, huffman_codes);

	cout << "Decoded string is:   " << decoded_string << endl;


	// The number of map bits becomes negligible for large encoded string length
	size_t num_map_bits = 0;

	for (auto pair : huffman_codes)
		num_map_bits += sizeof(char)*8 + pair.second.size(); // 8 bits per key + n bits per element

	size_t num_encoded_bits = encoded_string.size() + num_map_bits;
	size_t num_decoded_bits = decoded_string.size()*sizeof(char)*8;
	float scale = static_cast<float>(num_encoded_bits)/static_cast<float>(num_decoded_bits);

	// Scale is less than 1.0 if compression occurs
	cout << "Scale: " << scale << endl;

	clean_up();

	return 0;
}