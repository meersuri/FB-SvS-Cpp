#include<iostream>
#include<vector>
#include<fstream>
#include<random>
#include<omp.h>
#include<unordered_map>
#include"utils.h"

int main(int argc, char** argv)
{
	if(argc < 6)
	{
		std::cerr << "usage - " << argv[0] << "  " << "path to codebook" <<  "  " <<
		       	"path to ref seq"<< "  " << "number of queries" << 
		       "  " << 	"min query len" << "  " << "max query len" << std::endl; 
		return -1;
	}
	std::string codebook_file = argv[1];
	std::string ref_seq_file = argv[2];
	size_t n_queries = std::stoi(argv[3]);
	size_t min_query_len = std::stoi(argv[4]);
	size_t max_query_len = std::stoi(argv[5]);
	
	std::vector<char> alphabet; 
	std::string ref_seq;
	std::vector<std::string> codebook;
	std::unordered_map<std::string, std::vector<size_t>> index;
	std::vector<std::string> queries;

	load_codebook(codebook_file, codebook, alphabet);
	std::cout <<"finished loading codebook" << std::endl;
	auto max_code_it = std::max_element(codebook.begin(), codebook.end(), [] (std::string x1, std::string x2){ return x1.size() < x2.size();});
	size_t max_code_len = max_code_it->size();
	load_ref_seq(ref_seq_file, ref_seq);
	std::cout <<"finished loading ref sequence" << std::endl;

	size_t alphabet_size = alphabet.size();
	std::vector<float> symbol_probs(alphabet.size(), 0);
	calc_symbol_probs(ref_seq, alphabet, symbol_probs);
	std::default_random_engine generator;
	std::discrete_distribution<int> source_dist(symbol_probs.begin(), symbol_probs.end());
	build_index(ref_seq, codebook, index);
	std::cout <<"finished building index" << std::endl;
	std::uniform_int_distribution<int> query_len_dist(min_query_len, max_query_len);
	std::vector<std::pair<std::string, size_t>> failed_tests;
	#pragma omp parallel for
	for(int i = 0; i < n_queries; ++i)
	{
		size_t query_len = query_len_dist(generator);
		std::string query(query_len, alphabet[0]);
		for(int j = 0; j < query_len; ++j)
			query[j] = alphabet[source_dist(generator)];
		if((i % (int)(n_queries/10)) == 0)
			std::cout << "testing query " << i + 1 << " " << query << std::endl;
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
			failed_tests.push_back(std::make_pair(query, i));
	}
	std::cout << failed_tests.size() << " failed test cases" << std::endl;
	for(int i = 0; i < failed_tests.size(); ++i)
		std::cout << failed_tests[i].second << " ";
	std::cout << std::endl;
	return 0;
}

