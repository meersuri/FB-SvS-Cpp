#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<random>
#include<unordered_map>
#include<tuple>
#include"utils.h"

int main()
{
	size_t k = 3;
	const std::vector<char> alphabet = {'A', 'T', 'C', 'G'}; 
	size_t alphabet_size = alphabet.size();
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, alphabet_size - 1);
	size_t ref_seq_len = 1000;
	std::string ref_seq (ref_seq_len, alphabet[0]);
	for(int i = 0; i < ref_seq_len; ++i)
		ref_seq[i] = alphabet[distribution(generator)];
	for(int i = 0; i < ref_seq_len; ++i)
		std::cout << ref_seq[i];
	std::cout<<std::endl;
	std::unordered_map<std::string, std::vector<size_t>> index;
	std::vector<std::string> code;
	gen_k_grams_code(3, alphabet, code);
	build_index(ref_seq, code, index);

	size_t query_len = 100;
	std::string query(query_len, alphabet[0]);
	for(int i = 0; i < query_len; ++i)
		query[i] = alphabet[distribution(generator)];
	std::cout << query << std::endl;
	std::vector<std::tuple<std::string, size_t, size_t>> parsing_fwd;
	parse_query_fwd(query, index, parsing_fwd);
	for(int i = 0; i < parsing_fwd.size(); ++i)
		std::cout << std::get<0>(parsing_fwd[i]) << " " << std::get<1>(parsing_fwd[i]) << " " << std::get<2>(parsing_fwd[i]) << std::endl;
	std::cout << std::endl;
	std::vector<size_t> match_pos(index[std::get<0>parsing[0]]);
	for(int i = 1; i < parsing.size(); ++i)
	{
		std::vector<size_t> found;
		int d = std::get<2>(parsing[i]) - std::get<2>(parsing[0]);
		for(int j = 0; j < match_pos.size(); ++j)
		{
			std::vector<size_t>& posting_list = index[std::get<0>(parsing[i])];
			size_t key = match_pos[j] + d;
			auto it = std::lower_bound(posting_list.begin(), posting_list.end(), key);
			if(*it == key)
				found.push_back(match_pos[j]);
		}
		std::move(found.begin(), found.end(), match_pos.begin());
	}


