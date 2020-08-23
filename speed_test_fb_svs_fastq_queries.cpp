#include<iostream>
#include<vector>
#include<fstream>
#include<random>
#include<omp.h>
#include<chrono>
#include"utils.h"

int main(int argc, char** argv)
{
	using namespace std::chrono;
	if(argc < 4)
	{
		std::cerr << "usage - " << argv[0] << "  " << "path to codebook" <<  "  " << "path to index" << 
		       "  " << 	"path to queries" << std::endl; 
		return -1;
	}
	std::string codebook_file = argv[1];
	std::string index_file = argv[2];
	std::string queries_file = argv[3];
	
	std::vector<char> alphabet; 
	std::vector<std::string> codebook;
	std::unordered_map<std::string, std::vector<size_t>> index;
	std::vector<std::string> queries;

	load_codebook(codebook_file, codebook, alphabet);
	std::cout <<"finished loading codebook" << std::endl;
	auto max_code_it = std::max_element(codebook.begin(), codebook.end(), [] (std::string x1, std::string x2){ return x1.size() < x2.size();});
	size_t max_code_len = max_code_it->size();
	load_index(index_file, index);
	std::cout <<"finished loading index" << std::endl;
	load_queries(queries_file, queries);
	std::cout <<"finished loading queries" << std::endl;
	size_t n_queries = queries.size();
	steady_clock::time_point tp1 = steady_clock::now();
	#pragma omp parallel for
	for(int i = 0; i < n_queries; ++i)
	{
		std::string query = queries[i];
		if((i % (int)(n_queries/10)) == 0)
			std::cout << "testing query " << i + 1 << std::endl;
		std::vector<size_t> fb_svs_match_pos;
		int n_matches = fb_svs_k_grams(query, index, max_code_len, fb_svs_match_pos);
	}
	steady_clock::time_point tp2 = steady_clock::now();
	milliseconds delta = duration_cast<milliseconds>(tp2 - tp1);
	std::cout << "query time = " << delta.count() << " ms" << std::endl;
	return 0;
}

