#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<random>
#include<unordered_map>
#include<fstream>
#include"utils.h"

int main()
{
	std::vector<char> alphabet = {'A', 'T', 'C', 'G'}; 
	size_t alphabet_size = alphabet.size();
	std::ifstream ifs("k_grams_dna_8.txt");
	std::vector<std::string> codebook;
	char buffer[100];
	while(ifs.getline(buffer, 100))
		codebook.push_back(std::string(buffer));
	auto max_code_it = std::max_element(codebook.begin(), codebook.end(), [] (std::string x1, std::string x2){ return x1.size() < x2.size();});
	size_t max_code_len = max_code_it->size();

	ifs = std::ifstream("/mnt/d/ecoli.fasta");
	std::string ref_seq;
	while(ifs.getline(buffer, 100))
	{
		if(buffer[0] != '>')
			ref_seq += std::string(buffer);
	}
	std::cout << "reference sequence length = " << ref_seq.size() << std::endl;
	std::unordered_map<std::string, std::vector<size_t>> index;
	build_index(ref_seq, codebook, index);
	std::ofstream ofs("ecoli_k_grams_dna_8_index.txt");
	ofs << "index v1" << std::endl;
	for(auto it = index.begin(); it != index.end(); ++it)
	{
		ofs << it->first << std::endl;
		size_t posting_list_len = it->second.size();
		ofs << std::to_string(posting_list_len) << std::endl;
		for(int i = 0; i < posting_list_len; ++i)
			ofs << std::to_string(it->second[i]) << " ";
		ofs << std::endl;
	}
	ofs.close();
	return 0;
}
	       	
       	
