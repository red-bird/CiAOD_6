#include <iostream>
#include <list>
#include <map>
#include <string>

using namespace std;

struct Node
{
    char value;
	string code;
	int amount;
	Node* left;
	Node* right;

    Node(char value, int amount) {
        this->value = value;
        this->amount = amount;
        left = right = NULL;
    }

    Node(Node* left, Node* right) {
        value = 0;
        amount = left->amount + right->amount;
        this->left = left;
        this->right = right;
    }

    //bool operator < (const Node& tmp) const {
    //    return amount < tmp.amount;
    //}

    void traverse(string code = "") const {
        if (left != NULL) {
            left->traverse(code + '0');
            right->traverse(code + '1');
        }
        else
        {
            cout << "Value: " << value << " amount: " << amount;
        }
    }

};

struct cmp{ bool operator()(const Node* l, const Node* r) const { return l->amount < r->amount; }};
struct cmprev{ bool operator()(const Node* l, const Node* r) const { return l->amount > r->amount; } };

double getChance(const int& x, const int& size) {
    return (double)x / size;
}

void printAlphabet(map<char, int>& symbols, int length) {
    if (symbols.empty()) {
        cout << "  Alphabet empty\n";
        return;
    }
    map<char, int>::iterator iter;
    cout << "----------------------------------------\n  Alphabet\n  Char - amount - chance\n";
    for (iter = symbols.begin(); iter != symbols.end(); iter++)
    {
        cout << "  \'" << iter->first << "\'" << " - " << iter->second << " - " << getChance(iter->second, length) << endl;
    }
    cout << "----------------------------------------\n";
}

void printAlphabet(list<Node*> tmp, int length) {
    if (tmp.empty()) {
        cout << "  Alphabet empty\n";
        return;
    }
    list<Node*>::iterator iter;
    cout << "----------------------------------------\n  Alphabet sorted\n  Char - amount - chance\n";
    for (iter = tmp.begin(); iter != tmp.end(); iter++)
    {
        cout << "  \'" << (*iter)->value << "\'" << " - " << (*iter)->amount << " - " << getChance((*iter)->amount, length) << endl;
    }
    cout << "----------------------------------------\n";
}

list<Node*> convertMap(const map<char, int>& symbols) {
    list<Node*> tmp;
    for (auto itr = symbols.begin(); itr != symbols.end(); ++itr)
    {
        Node* p = new Node(itr->first, itr->second);
        tmp.push_back(p);
    }
    return tmp;
}

Node* buildTree(list<Node*> tmp) {
    while (tmp.size() != 1)
    {
        tmp.sort(cmp());

        Node* ChildL = tmp.front();
        tmp.pop_front();
        Node* ChildR = tmp.front();
        tmp.pop_front();

        Node* parent = new Node(ChildL, ChildR);
        tmp.push_back(parent);
    }
    return tmp.front(); // return root
}

void buildTable(Node* root, map<char, string> &tmp) {
    if (root->left != NULL) { root->left->code.insert(0,root->code); root->left->code.push_back('0'); buildTable(root->left, tmp); }
    if (root->right != NULL) { root->right->code.insert(0, root->code);; root->right->code.push_back('1'); buildTable(root->right, tmp); }
    if (root->value) tmp[root->value] = root->code;
}

void printTable(map<char, string>& tmp) {
    map<char, string>::iterator iter;
    cout << "----------------------------------------\n Table of codes\n Char : code\n";
    if (tmp.size() == 1)
        cout << "  " << tmp.begin()->first << " : " << "0\n";
    else {
        for (iter = tmp.begin(); iter != tmp.end(); iter++) {
            cout << "  " << iter->first << " : " << iter->second << endl;
        }
    }
    cout << "----------------------------------------\n";
}

void printTree(const Node* root, unsigned depth = 0) {
    if (root != NULL) {
        printTree(root->left, ++depth);

        for (unsigned i = 0; i < depth; i++)
            cout << "  .";

        if (root->value)
            cout << " (" << root->value << ")\n";
        else
            cout<< endl;
        printTree(root->right, ++depth);
    }
}

void printTreeInfo(const Node* root, unsigned depth = 0) {
    cout << "----------------------------------------\n  Description: dots - depth level of Node, chars in the end - leaves\n";
    cout << "  Tree hierarchy:\n";
    printTree(root);
    cout << "----------------------------------------\n";
}

string getCompressedString(map<char, string>& tmp, string input) {
    string output;
    for (int i = 0; i < input.length(); i++)
    {
        output.append(tmp[input[i]]);
    }
    return output;
}

void printCompressedString(string binary, map<char, string>& tmp, string input) {
    cout << "----------------------------------------\n  Compressed string:\n  ";
    if (input.length() == 1)
        cout << "0\n";
    else if (tmp.size() == 1) {
        for (int i = 0; i < input.length(); i++)
        {
            cout << "0";
        }
        cout << "\n";
    }
    else
        cout << getCompressedString(tmp, input) << endl;
    cout << "----------------------------------------\n";
}

void printCoefficient(string binary, string input, map<char, string>& tmp) {
    if ((input.length() == 1) || tmp.size() == 1) {
        cout << "----------------------------------------\n  Compress Coefficient\n";
        cout << "  ASCII / Huff = 8" << endl;
        cout << "  Evanly / Huff = 1" << endl;
        cout << "----------------------------------------\n";
    }
    else {
        cout << "----------------------------------------\n  Compress Coefficient\n";
        cout << "  ASCII / Huff =  " << (double)(input.length() * 8) / binary.length() << endl;
        string max = "";
        for (auto iter = tmp.begin(); iter != tmp.end(); iter++) { if (iter->second.length() > max.length()) { max = iter->second; } }
        cout << "  Evenly / Huff =  " << (double)(max.length() * input.length()) / binary.length() << endl;
        cout << "----------------------------------------\n";
    }
}

void printCalculatios(string binary, string input, map<char, string> &codeTable, map<char, int> &alphabetTable) {
    double average = 0, disspersion = 0;
    cout << "----------------------------------------\n  Average :  ";
    if ((input.length() == 1) || (codeTable.size() == 1)) {
        cout << "1" << endl;
        cout << "  Disspersion : 0\n";
    }
    else {
        for (auto iter = alphabetTable.begin(); iter != alphabetTable.end(); iter++)
        {
            average += (getChance((iter->second), input.length()) * codeTable[iter->first].length());
        }
        cout << average << endl;
        
        for (auto iter = alphabetTable.begin(); iter != alphabetTable.end(); iter++) {
            disspersion += (getChance((iter->second), input.length()) * (codeTable[iter->first].length() - average) * (codeTable[iter->first].length() - average));
        }
        cout << "  Disspersion : " << disspersion << endl;

    }
    cout << "----------------------------------------\n";
}

int main() {
	cout << "Enter string:\n";
	string input;
	getline(cin, input);
    if (input.length() < 1) { cout << "incorrect input string\n"; return -1; }
	map<char, int> symbols;
    
	for (int i = 0; i < input.length(); i++)
		symbols[input[i]]++;
	
    printAlphabet(symbols, input.length());

    list<Node*> ls = convertMap(symbols);
    ls.sort(cmprev());
    printAlphabet(ls, input.length());

    Node* root = buildTree(ls);

    printTreeInfo(root);

    map<char, string> table;
    buildTable(root, table);
    printTable(table);

    string compress = getCompressedString(table, input);
    printCompressedString(compress, table, input);
    printCoefficient(compress, input, table);
    printCalculatios(compress, input, table, symbols);

}