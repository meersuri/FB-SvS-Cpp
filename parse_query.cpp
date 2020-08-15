
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
	std::cout << "referece sequence -" << std::endl;
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
	std::cout << "query sequence -" << std::endl;
	std::cout << query << std::endl;
	std::vector<std::tuple<std::string, size_t, size_t>> parsing_fwd;
	parse_query_fwd(query, index, parsing_fwd);
	std::cout << "query forward parse - (code word, posting list length, index)" << std::endl;
	for(int i = 0; i < parsing_fwd.size(); ++i)
		std::cout << std::get<0>(parsing_fwd[i]) << " " << std::get<1>(parsing_fwd[i]) << " " << std::get<2>(parsing_fwd[i]) << std::endl;
	std::cout << std::endl;
	std::vector<std::tuple<std::string, size_t, size_t>> parsing_bwd;
	parse_query_bwd(query, index, parsing_bwd);
	std::cout << "query backward parse - (code word, posting list length, index)" << std::endl;
	for(int i = 0; i < parsing_bwd.size(); ++i)
		std::cout << std::get<0>(parsing_bwd[i]) << " " << std::get<1>(parsing_bwd[i]) << " " << std::get<2>(parsing_bwd[i]) << std::endl;
	std::cout << std::endl;
	return 0;
}
