#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<random>
#include<unordered_map>
#include<tuple>
#include<fstream>
#include"Node.h"
#include"utils.h"

Node::Node()
{
	for(int i = 0; i < Node::alphabet.size(); ++i)
		children.push_back(nullptr);
}

Node::Node(char node_val): val(node_val)
{
	for(int i = 0; i < Node::alphabet.size(); ++i)
		children.push_back(nullptr);
}

void Node::extend()
{
	bool to_extend = Node::distribution(Node::generator);
	if(to_extend)
	{

		for(int i = 0; i < Node::alphabet.size(); ++i)
		{
			if(to_extend)
				children[i] = new Node(Node::alphabet[i]);
		}
	}
}

void grow_tree(Node* node, size_t depth, size_t max_depth)
{
	//std::cout << node->val << " " << depth << std::endl;
	if(node == nullptr)
		return;
	if(depth == max_depth)
		return;
	node->extend();
	for(int i = 0; i < node->children.size(); ++i)
	{
		if(node->children[i] != nullptr)
			grow_tree(node->children[i], depth + 1, max_depth);
	}
	return;
}	

void traverse_tree(Node* node, std::string& code, std::vector<std::string>& codewords)
{
	if(node == nullptr)
		return;
	code.push_back(node->val);
	bool is_leaf = true;
	for(int i = 0; i < node->children.size(); ++i)
	{
		if(node->children[i] != nullptr)
		{
			is_leaf = false;
			traverse_tree(node->children[i], code, codewords);
		}
	}
	if(is_leaf)
		codewords.push_back(code.substr(1, code.size() - 1));
	code.pop_back();
	return;
}

void gen_prefix_code(size_t max_code_len, std::vector<std::string>& codebook)
{
	Node* root = new Node('X');
	for(int i =0; i < root->alphabet.size(); ++i)
	{
		root->children[i] = new Node(root->alphabet[i]);
		grow_tree(root->children[i], 0, max_code_len);
	}
	std::string code;
	traverse_tree(root, code, codebook);
	return;
}

std::default_random_engine Node::generator;
std::vector<char> Node::alphabet = std::vector<char>{'A', 'C', 'G', 'T'};
//std::vector<char> Node::alphabet = std::vector<char>{'A', 'C', 'G', 'T', 'N'};
//std::vector<char> Node::alphabet = std::vector<char>{'U', 'T', 'A', 'W', 'C', 'Y', 'M', 'H', 'G', 'K', 'R', 'D', 'S', 'B', 'V', 'N'};
std::discrete_distribution<int> Node::distribution = std::discrete_distribution<int>{0.10, 0.90};


int main(int argc, char** argv)
{
	if(argc < 5)
	{
		std::cerr << "usage - " << argv[0] << "  " << "alphabet" << "  " << "node split prob" <<
		       	"  " << "max codeword length" << "  " << "output file" << std::endl;
		return -1;
	}
	std::vector<char> alphabet;
	std::string alphabet_string = argv[1];
	for(int i = 0; i < alphabet_string.size(); ++i)
		alphabet.push_back(alphabet_string[i]);
	float node_split_prob = std::stof(argv[2]);
	size_t max_depth = std::stoi(argv[3]);
	std::string out_file = argv[4];
	
	Node::alphabet = alphabet;
	Node::distribution = std::discrete_distribution<int>{1 - node_split_prob, node_split_prob};

	std::vector<std::string> codebook;
	gen_prefix_code(max_depth, codebook);
	std::cout << "codebook size = " << codebook.size() << std::endl;
	std::ofstream ofs("codebook_dna_5.txt");
	write_codebook(out_file, codebook, alphabet);
	return 0;
}
