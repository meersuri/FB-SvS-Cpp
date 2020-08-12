#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<random>
#include<unordered_map>
#include"utils.h"

int main()
{
	size_t k = 3;
	std::vector<char> alphabet = {'A', 'T', 'C', 'G'}; 
	size_t alphabet_size = alphabet.size();
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, alphabet_size - 1);
	size_t ref_seq_len = 1000;
	std::string ref_seq (ref_seq_len, alphabet[0]);
	for(int i = 0; i < ref_seq_len; ++i)
		ref_seq[i] = alphabet[distribution(generator)];
	for(int i = 0; i < ref_seq_len; ++i)
		std::cout << ref_seq[i];
	std::cout<<std::endl;
	std::unordered_map<std::string, std::vector<size_t>> index;
	std::vector<std::string> code;
	gen_k_grams_code(3, alphabet, code);
	build_index(ref_seq, code, index);
	for(auto it = index.begin(); it != index.end(); ++it)
	{
		std::vector<size_t>& vec = it->second;
		std::cout << it->first << " " << vec.size() << std::endl; 
		for(int i = 0; i < vec.size(); ++i)
			std::cout << vec[i] << " ";
		std::cout << std::endl;
	}
	return 0;
}
	       	
       	
