#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include"utils.h"

int main()
{
	size_t k = 3;
	std::vector<char> alphabet = {'A', 'T', 'C', 'G'};
	std::vector<std::string> code;
	gen_k_grams_code(k, alphabet, code);
	std::cout << "k-gram code words - " << std::endl;
	for(int i = 0; i < code.size(); ++i)
		std::cout << code[i] << std::endl;
	return 0;
}

