#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<random>
#include<unordered_map>
#include<tuple>
#include<omp.h>
#include"utils.h"

int main(int argc, char** argv)
{
	if(argc < 7)
	{
		std::cerr << "usage - " << argv[0] << "  " << "kmer size" <<  "  " <<
		       	"alphabet" << "  "  << "ref sequence len" << 
			"  " <<  "number of queries" << "  " << "min query len" <<
		       	"  " << "max query len" << std::endl; 
		return -1;
	}
	size_t k = std::stoi(argv[1]);
	std::string alphabet_str = argv[2];
	size_t ref_seq_len = std::stoi(argv[3]);
	size_t n_queries = std::stoi(argv[4]);
	size_t min_query_len = std::stoi(argv[5]);
	size_t max_query_len = std::stoi(argv[6]);

	std::vector<char> alphabet;
	for(int i = 0; i < alphabet_str.size(); ++i)
		alphabet.push_back(alphabet_str[i]);

	size_t alphabet_size = alphabet.size();
	std::default_random_engine generator;
	std::uniform_int_distribution<int> source_dist(0, alphabet_size - 1);
	std::string ref_seq(ref_seq_len, alphabet[0]);
	for(int i = 0; i < ref_seq_len; ++i)
		ref_seq[i] = alphabet[source_dist(generator)];
	std::unordered_map<std::string, std::vector<size_t>> index;
	std::vector<std::string> codebook;
	gen_k_grams_code(k, alphabet, codebook);
	auto max_code_it = std::max_element(codebook.begin(), codebook.end(), [] (std::string x1, std::string x2){ return x1.size() < x2.size();});
	size_t max_code_len = max_code_it->size();
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
