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
	size_t max_query_len = 2000;
	size_t min_query_len = 1000;
	size_t n_queries = 2000;
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
			std::cout << "testing query " << i + 1 << std::endl;
		std::vector<size_t> fb_svs_match_pos;
		//int n_matches = fb_svs(query, index, fb_svs_match_pos);
		std::vector<size_t> str_find_match_pos;
		for(int i = 0; i < ref_seq.size() - query.size(); ++i)
		{
			bool match = true;
			for(int j = 0; j < query.size(); ++j)
			{
				if(query[j] != ref_seq[j + i])
				{
					match = false;
					break;
				}
			}
			if(match)
				str_find_match_pos.push_back(i);
		}
	}
	return 0;
}

