#include<iostream>
#include<vector>
#include<fstream>
#include<random>
#include<omp.h>
#include"utils.h"

int main()
{
	std::ifstream ifs("k_grams_dna_7.txt");
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
	std::cout <<"finished building index" << std::endl;
	ifs = std::ifstream("./ecoli_reads_R1.fastq");
	char line[500];
	std::vector<std::string> queries;
	bool start_seq = false;
	while(ifs.getline(line, 500))
	{
		if(start_seq)
		{
			queries.push_back(std::string(line));
			start_seq = false;
		}
		if(line[0] == '@')
			start_seq = true;

	}
	size_t n_queries = queries.size();
	std::vector<std::pair<std::string, size_t>> failed_tests;
//	#pragma omp parallel for
	for(int i = 0; i < n_queries; ++i)
	{
		std::string query = queries[i];
		if((i % 100) == 0)
			std::cout << "testing query " << i + 1 << std::endl;
		std::vector<size_t> fb_svs_match_pos;
		int n_matches = fb_svs(query, index, max_code_len, fb_svs_match_pos);
		std::vector<size_t> str_find_match_pos;
		size_t start = 0;
		while(1)
		{
			size_t res = ref_seq.find(query, start);
			if (res != std::string::npos)
			{
				str_find_match_pos.push_back(res);
				start = res + 1;
			}
			else
				break;
		}
		if(fb_svs_match_pos != str_find_match_pos)
//			std::cout << "failed test case - " << query << std::endl;
			failed_tests.push_back(std::make_pair(query, i));
	}
	std::cout << failed_tests.size() << " failed test cases" << std::endl;
	for(int i = 0; i < failed_tests.size(); ++i)
		std::cout << failed_tests[i].second << " ";
	std::cout << std::endl;
	return 0;
}

