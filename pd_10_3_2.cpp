#include <iostream>
#include <queue>
#include <map>
#include <vector>
using namespace std;


// Modified from ChatGPT code
struct HuffmanNode
{
    char data;
    int frequency;

    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr)
    {
    
    }
};

vector<HuffmanNode*> pointers_to_clean_up;

struct CompareNodes {
    bool operator()(HuffmanNode* lhs, HuffmanNode* rhs)
    {
        return lhs->frequency > rhs->frequency;
    }
};

HuffmanNode* buildHuffmanTree(map<char, int>& frequencies)
{
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> pq;

    for (const auto& entry : frequencies)
    {
        HuffmanNode* t = new HuffmanNode(entry.first, entry.second);
        pointers_to_clean_up.push_back(t);
        pq.push(t);
    }

    while (pq.size() > 1)
    {
        HuffmanNode* left = pq.top();
        pq.pop();

        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* newNode = new HuffmanNode('\0', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;
        pointers_to_clean_up.push_back(newNode);
        pq.push(newNode);
    }

    return pq.top();
}

void generateCodes(HuffmanNode* root, string code, map<char, string>& codes)
{
    if (root == nullptr) 
        return;

    if (root->data != '\0')
        codes[root->data] = code;

    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

string encode(string message, map<char, string>& codes)
{
    string encodedMessage = "";

    for (char c : message)
        encodedMessage += codes[c];

    return encodedMessage;
}

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

        if (current->left == nullptr && current->right == nullptr)
        {
            decodedMessage += current->data;
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



int main(void) 
{
    string message = "this is an example for huffman encoding";

    map<char, int> frequencies;

    for (char c : message)
        frequencies[c]++;

    HuffmanNode* root = buildHuffmanTree(frequencies);

    map<char, string> codes;
    generateCodes(root, "", codes);

    string encodedMessage = encode(message, codes);
    cout << "Encoded Message: " << encodedMessage << endl;

    string decodedMessage = decode(root, encodedMessage);
    cout << "Decoded Message: " << decodedMessage << endl;

    clean_up();

    return 0;
}   