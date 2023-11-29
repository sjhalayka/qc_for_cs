// https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1186/assn/huffman.html



#include <iostream>
#include <queue>
#include <map>
#include <vector>

using namespace std;

// Define a structure for a Huffman tree node
struct HuffmanNode
{
    char datum;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char datum, int frequency) : datum(datum), frequency(frequency), left(nullptr), right(nullptr) {}
};

vector<HuffmanNode*> pointers_to_clean_up;

// Comparison function for priority queue
struct CompareNodes
{
    bool operator()(HuffmanNode* lhs, HuffmanNode* rhs)
    {
        return lhs->frequency > rhs->frequency;
    }
};

// Function to build the Huffman tree
HuffmanNode* buildHuffmanTree(map<char, int>& frequencies) 
{
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (const auto& entry : frequencies)
    {
        HuffmanNode* t = new HuffmanNode(entry.first, entry.second);
        pointers_to_clean_up.push_back(t);
        pq.push(t);
    }

    // Build the Huffman tree by repeatedly combining the least frequent nodes
    while (pq.size() > 1) 
    {
        HuffmanNode* left = pq.top();
        pq.pop();

        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* newNode = new HuffmanNode('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;
        pointers_to_clean_up.push_back(newNode);
        pq.push(newNode);
    }

    // The remaining node is the root of the Huffman tree
    return pq.top();
}

// Function to traverse the Huffman tree and generate Huffman codes
void generateCodes(HuffmanNode* root, string code, map<char, string>& codes)
{
    if (root == nullptr)
        return;

    if (root->datum != '$')
        codes[root->datum] = code;

    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

// Function to encode a message using Huffman codes
string encode(string message, map<char, string>& codes)
{
    string encodedMessage = "";

    for (char c : message)
        encodedMessage += codes[c];

    return encodedMessage;
}

// Function to decode a message using Huffman codes
string decode(HuffmanNode* root, string encodedMessage)
{
    string decodedMessage = "";
    HuffmanNode* current = root;

    for (char bit : encodedMessage) 
    {
        if (bit == '0') 
            current = current->left;
        else 
            current = current->right;

        if (current->datum != '$') 
        {
            decodedMessage += current->datum;
            current = root;
        }
    }

    return decodedMessage;
}

void clean_up(void)
{
    for (size_t i = 0; i < pointers_to_clean_up.size(); i++)
        delete pointers_to_clean_up[i];
}

int main()
{
    string message = "This is an example for huffman encoding!";

    // Count the frequency of each character in the message
    map<char, int> frequencies;

    for (char c : message)
        frequencies[c]++;

    // Build the Huffman tree
    HuffmanNode* root = buildHuffmanTree(frequencies);

    // Generate Huffman codes for each character
    map<char, string> codes;
    generateCodes(root, "", codes);

    // Encode the message
    string encodedMessage = encode(message, codes);
    cout << "Encoded Message: " << encodedMessage << endl;

    // Decode the message
    string decodedMessage = decode(root, encodedMessage);
    cout << "Decoded Message: " << decodedMessage << endl;

    clean_up();

    return 0;
}