#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;


// See:
// https://gist.github.com/pwxcoo/72d7d3c5c3698371c21e486722f9b34b
// https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
// https://www.programiz.com/dsa/huffman-coding
// https://iq.opengenus.org/huffman-encoding/


struct Node
{
	char ch;
	int freq;
	Node *left, *right;
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
void encode(Node* root, string str,
	unordered_map<char, string>& huffmanCode)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right)
		huffmanCode[root->ch] = str;

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

// traverse the Huffman Tree and decode the encoded string
void decode(Node* root, int& index, string str)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right)
	{
		cout << root->ch;
		return;
	}

	index++;

	if (str[index] == '0')
		decode(root->left, index, str);
	else
		decode(root->right, index, str);
}

void clean_up(void)
{
	cout << "Cleaning up " << nodes_to_clean_up.size() << " nodes." << endl;

	for (size_t i = 0; i < nodes_to_clean_up.size(); i++)
		delete nodes_to_clean_up[i];
}


void get_encoded_and_codes(const string &input, string& encoded_string, unordered_map<char, string>& um, Node* &root)
{

	// count frequency of appearance of each character
	// and store it in a map
	unordered_map<char, int> freq;

	for (char ch : input)
		freq[ch]++;

	if (freq.size() == 1)
	{
		//string encode;

		//const size_t num_zeroes = freq.begin()->second;

		//for (size_t i = 0; i < num_zeroes; i++)
		//	encode += '0';

		//return encode;
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
	string text = "AAAAAB";

	string encoded_string;
	unordered_map<char, string> huffmanCode;
	Node* root = nullptr;

	get_encoded_and_codes(text, encoded_string, huffmanCode, root);

	cout << "Huffman Codes are :\n" << '\n';
	for (auto pair : huffmanCode) {
		cout << pair.first << " " << pair.second << '\n';
	}

	cout << "\nOriginal string was :\n" << text << '\n';

	// print encoded string
	string str = "";
	for (char ch : text) {
		str += huffmanCode[ch];
	}


	cout << "\nEncoded string is :\n" << str << '\n';

	// traverse the Huffman Tree again and this time
	// decode the encoded string
	int index = -1;
	cout << "\nDecoded string is: \n";
	while (index < (int)str.size() - 1) {
		decode(root, index, str);
	}

	cout << endl;

	clean_up();

	return 0;
}