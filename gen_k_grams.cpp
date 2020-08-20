#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<fstream>
#include"utils.h"

int main()
{
	size_t k = 8;
	std::vector<char> alphabet = {'A', 'T', 'C', 'G'};
	std::vector<std::string> codebook;
	gen_k_grams_code(k, alphabet, codebook);
//	std::cout << "k-gram code words - " << std::endl;
//	for(int i = 0; i < code.size(); ++i)
//		std::cout << code[i] << std::endl;
	std::cout << "codebook size = " << codebook.size() << std::endl;
	std::ofstream ofs("k_grams_dna_" + std::to_string(k) + ".txt");
	for(int i =0; i < codebook.size(); ++i)
		ofs << codebook[i] << std::endl;
	return 0;
}

