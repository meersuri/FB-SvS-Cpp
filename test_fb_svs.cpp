#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<random>
#include<unordered_map>
#include<tuple>
#include<omp.h>
#include"utils.h"

int main()
{
	size_t k = 4;
	const std::vector<char> alphabet = {'A', 'T', 'C', 'G'}; 
	size_t alphabet_size = alphabet.size();
	std::default_random_engine generator;
	std::uniform_int_distribution<int> source_dist(0, alphabet_size - 1);
	size_t ref_seq_len = 10000000;
	std::string ref_seq (ref_seq_len, alphabet[0]);
	for(int i = 0; i < ref_seq_len; ++i)
		ref_seq[i] = alphabet[source_dist(generator)];
	std::unordered_map<std::string, std::vector<size_t>> index;
	std::vector<std::string> code;
	gen_k_grams_code(k, alphabet, code);
	auto max_code_it = std::max_element(code.begin(), code.end(), [] (std::string x1, std::string x2){ return x1.size() > x2.size();});
	size_t max_code_len = max_code_it->size();
	build_index(ref_seq, code, index);

	size_t max_query_len = 100;
	size_t min_query_len = 1;
	size_t n_queries = 1000;
	std::uniform_int_distribution<int> query_len_dist(min_query_len, max_query_len);
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
			std::cout << "failed test case - " << query << std::endl;
	}
	return 0;
}
