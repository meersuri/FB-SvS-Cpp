#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<random>
#include<unordered_map>
#include<tuple>
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
	for(int i = 0; i < Node::alphabet.size(); ++i)
	{
		bool to_extend = Node::distribution(Node::generator);
		if(to_extend)
			children[i] = new Node(Node::alphabet[i]);
	}
}

std::default_random_engine Node::generator;
std::vector<char> Node::alphabet = std::vector<char>{'A', 'C', 'G', 'T'};
std::discrete_distribution<int> Node::distribution = std::discrete_distribution<int>{0.5, 0.5};

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
		
int main()
{
	size_t max_depth = 5;
	Node* root = new Node('X');
	grow_tree(root, 0, max_depth);
	std::string code;
	std::vector<std::string> codewords;
	traverse_tree(root, code, codewords);
        for(int i =0; i < codewords.size(); ++i)
		std::cout << codewords[i] << std::endl;
	std::cout << std::endl;	
	return 0;
}
