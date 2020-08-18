#include<iostream>
#include<vector>
#include<fstream>
#include<random>
#include<omp.h>
#include"utils.h"

int main()
{
	std::ifstream ifs("codebook_dna_3.txt");
	std::vector<std::string> codebook;
	char buffer[100];
	while(ifs.getline(buffer, 100))
		codebook.push_back(std::string(buffer));

	ifs = std::ifstream("/mnt/d/ecoli.fasta");
	std::string ref_seq;
	while(ifs.getline(buffer, 100))
	{
		if(buffer[0] != '>')
			ref_seq += std::string(buffer);
	}
	std::cout << "reference sequence length = " << ref_seq.size() << std::endl;
	std::cout << ref_seq.substr(0, 140) << std::endl;
	const std::vector<char> alphabet{'A', 'C', 'G', 'T'}; 
	size_t alphabet_size = alphabet.size();
	std::default_random_engine generator;
	std::uniform_int_distribution<int> source_dist(0, alphabet_size - 1);
	std::unordered_map<std::string, std::vector<size_t>> index;
	build_index(ref_seq, codebook, index);
	std::cout <<"finished building index" << std::endl;
//	std::string query = ref_seq.substr(3141, query_len);
	size_t max_query_len = 1000;
	size_t min_query_len = 20;
	size_t n_queries = 1000;
	std::uniform_int_distribution<int> query_len_dist(min_query_len, max_query_len);
	std::vector<std::string> failed_tests;
	#pragma omp parallel for
	for(int i = 0; i < n_queries; ++i)
	{
		size_t query_len = query_len_dist(generator);
		std::string query(query_len, alphabet[0]);
		for(int j = 0; j < query_len; ++j)
			query[j] = alphabet[source_dist(generator)];
		if((i % 100) == 0)
			std::cout << "testing query " << i + 1 << " " << query << std::endl;
		std::vector<size_t> fb_svs_match_pos;
		int n_matches = fb_svs(query, index, fb_svs_match_pos);
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
		{
//			std::cout << "failed test case - " << query << std::endl;
			failed_tests.push_back(query);
		}
	}
	std::cout << failed_tests.size() << " failed test cases" << std::endl;
	for(int i = 0; i < failed_tests.size(); ++i)
		std::cout << failed_tests[i] << std::endl;
	return 0;
}

