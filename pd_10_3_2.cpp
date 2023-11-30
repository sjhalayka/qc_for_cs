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
Node* getNode(char ch, int freq, Node* left, Node* right)
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
void encode(Node* root, const string str, unordered_map<char, string>& huffmanCode)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right)
		huffmanCode[root->ch] = str;

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

string str_trim(string& str, size_t pos)
{
	string temp = "";

	for (size_t i = pos; i < str.size(); i++)
		temp += str[i];

	return temp;
}

void decode(string encoded_string, string& decoded_string, const unordered_map<char, string>& huffman_codes)
{
	if (huffman_codes.size() == 1)
	{
		const char c = huffman_codes.begin()->first; // should be 0

		for (size_t i = 0; i < encoded_string.size(); i++)
			decoded_string += c;

		return;
	}

	decoded_string = "";

	while (encoded_string != "")
	{
		size_t end = 0;

		while (end < encoded_string.size())
		{
			bool abort = false;

			string token = encoded_string.substr(0, end + 1);
			
			for (auto pair : huffman_codes)
			{
				if (pair.second == token)
				{
					decoded_string += pair.first;
					encoded_string = str_trim(encoded_string, token.length());
					abort = true;
					break;
				}
			}

			if (abort)
				break;
			else
				end++;
		}
	}
}

void clean_up(void)
{
	cout << "Cleaning up " << nodes_to_clean_up.size() << " nodes." << endl;

	for (size_t i = 0; i < nodes_to_clean_up.size(); i++)
		delete nodes_to_clean_up[i];
}

void get_codes(const string& input, unordered_map<char, string>& um)
{
	Node *root = nullptr;

	// count frequency of appearance of each character
	// and store it in a map
	unordered_map<char, int> freq;

	for (char ch : input)
		freq[ch]++;

	if (freq.size() == 1)
	{
		root = getNode(freq.begin()->first, freq.begin()->second, nullptr, nullptr);
		um[root->ch] = "0";

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

	encode(root, "", um);
}


int main()
{
	// Strings with low entropy produce higher compression rates
	//string text = "AAAAAAAAAAAAAAAAAAAAAAAAAA"; // Compression 87.5%
	string text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // Compression 40.4%

	unordered_map<char, string> huffman_codes;
	get_codes(text, huffman_codes);

	cout << "Huffman codes:" << endl;

	for (auto pair : huffman_codes)
		cout << pair.first << " " << pair.second << endl;

	cout << endl;


	cout << "Original string was: " << text << endl;

	// print encoded string
	string str = "";

	for (char ch : text)
		str += huffman_codes[ch];

	cout << "Encoded string is:   " << str << endl;


	string decoded_string;

	decode(str, decoded_string, huffman_codes);

	cout << "Decoded string is:   " << decoded_string << endl;


	// Note: this does not take into account the size of the
	// Huffman codes map<char, string>, but it's a close estimate
	size_t num_encoded_bits = str.size();
	size_t num_decoded_bits = decoded_string.size() * sizeof(char) * 8;
	float compression = 1.0f - static_cast<float>(num_encoded_bits) / static_cast<float>(num_decoded_bits);

	cout << "Compression rate: " << compression * 100.0f << "%" << endl;

	clean_up();

	return 0;
}