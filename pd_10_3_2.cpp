#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <algorithm>
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
void decode(Node* root, int& index, string str, string &decoded_string)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right)
	{
		decoded_string += root->ch;
		return;
	}

	index++;

	if (str[index] == '0')
		decode(root->left, index, str, decoded_string);
	else
		decode(root->right, index, str, decoded_string);
}


string Left(string& str, int pos)
{
	int i;
	string temp = "";
	for (i = 0; i < pos; i++)
	{
		temp += str[i];
	}

	return temp;
}

string Right(string& str, int pos)
{
	int i;
	string temp = "";
	for (i = pos; i < strlen(str.c_str()); i++)
	{
		temp += str[i];
	}
	return temp;
}

void decode2(string encoded_string, string& decoded_string, const unordered_map<char, string>& huffman_codes)
{
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
					encoded_string = Right(encoded_string, token.length());// encoded_string.substr(end, encoded_string.length() - token.length());
					abort = true;

					cout << encoded_string << endl;

					break;
				}
			}

			if (abort)
				break;

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

void get_codes(const string& input, unordered_map<char, string>& um, Node*& root)
{
	root = nullptr;

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
	string text = "BDFAAAAABCG";

	//string encoded_string;
	unordered_map<char, string> huffman_codes;
	Node* root = nullptr;

	get_codes(text, huffman_codes, root);

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

	// traverse the Huffman Tree again and this time
	// decode the encoded string

	cout << "Decoded string is:   ";
	string decoded_string;

	if (huffman_codes.size() == 1)
	{
		const char c = huffman_codes.begin()->first;

		for (size_t i = 0; i < str.size(); i++)
			decoded_string += c;
	}
	else
	{
		decode2(str, decoded_string, huffman_codes);
		
		/*
		int index = -1;

		while (index < (int)str.size() - 1)
			decode(root, index, str, decoded_string);	
		*/
	}

	cout << decoded_string << endl;

	clean_up();

	return 0;
}