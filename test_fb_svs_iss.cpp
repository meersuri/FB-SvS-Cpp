#include<iostream>
#include<vector>
#include<fstream>
#include<random>
#include<omp.h>
#include"utils.h"

int main(int argc, char** argv)
{
	if(argc < 5)
	{
		std::cerr << "usage - " << argv[0] << "  " << "path to codebook" <<  "  " <<
		       	"path to ref seq"<< "  " << "path to index" << 
		       "  " << 	"path to queries" << std::endl; 
		return -1;
	}
	std::string codebook_file = argv[1];
	std::string ref_seq_file = argv[2];
	std::string index_file = argv[3];
	std::string queries_file = argv[4];
	
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
	load_index(index_file, index);
	std::cout <<"finished loading index" << std::endl;
	load_queries(queries_file, queries);
	std::cout <<"finished loading queries" << std::endl;
	size_t n_queries = queries.size();
	std::vector<std::pair<std::string, size_t>> failed_tests;
	#pragma omp parallel for
	for(int i = 0; i < n_queries; ++i)
	{
		std::string query = queries[i];
		if((i % 10000) == 0)
			std::cout << "testing query " << i + 1 << std::endl;
		std::vector<size_t> fb_svs_match_pos;
		int n_matches = fb_svs_k_grams(query, index, max_code_len, fb_svs_match_pos);
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

