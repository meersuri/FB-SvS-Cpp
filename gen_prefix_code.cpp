#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<random>
#include<unordered_map>
#include<tuple>
#include<fstream>
#include"Node.h"

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

std::default_random_engine Node::generator;
std::vector<char> Node::alphabet = std::vector<char>{'A', 'C', 'G', 'T', 'N'};
//std::vector<char> Node::alphabet = std::vector<char>{'U', 'T', 'A', 'W', 'C', 'Y', 'M', 'H', 'G', 'K', 'R', 'D', 'S', 'B', 'V', 'N'};
std::discrete_distribution<int> Node::distribution = std::discrete_distribution<int>{0.55, 0.45};

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

int main()
{
	size_t max_depth = 6;
	std::vector<std::string> codebook;
	gen_prefix_code(max_depth, codebook);
	std::cout << "codebook size = " << codebook.size() << std::endl;
	std::ofstream ofs("codebook_dna_1.txt");
	for(int i =0; i < codebook.size(); ++i)
		ofs << codebook[i] << std::endl;
//        for(int i =0; i < codewords.size(); ++i)
//		std::cout << codewords[i] << std::endl;
//	std::cout << std::endl;	
//	
	return 0;
}
