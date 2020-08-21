#include<unordered_map>
#include<vector>
#include<fstream>
#include<string>
#include<iostream>
#include"utils.h"

int main()
{

	std::unordered_map<std::string, std::vector<size_t>> index;
	load_index("ecoli_k_grams_dna_8_index.txt", index);
	std::cout << "index size = " << index.size() << std::endl;
	return 0;
}



