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
	size_t ref_seq_len = 50000;
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

	size_t query_len = 7;
	std::string query(query_len, alphabet[0]);
	for(int i = 0; i < query_len; ++i)
		query[i] = alphabet[distribution(generator)];
	//query = "TTTTTTAT";
	std::cout << "query sequence -" << std::endl;
	std::cout << query << std::endl;
	std::vector<std::tuple<std::string, size_t, size_t>> parsing_fwd;
	std::vector<std::tuple<std::string, size_t, size_t>> parsing_bwd;
	size_t fwd_parse_start = parse_query_fwd(query, index, parsing_fwd);
	std::cout << "query forward parse - (code word, posting list length, index)" << std::endl;
	for(int i = 0; i < parsing_fwd.size(); ++i)
		std::cout << std::get<0>(parsing_fwd[i]) << " " << std::get<1>(parsing_fwd[i]) << " " << std::get<2>(parsing_fwd[i]) << std::endl;
	size_t bwd_parse_start = parse_query_bwd(query, index, parsing_bwd);
	std::cout << "query backward parse - (code word, posting list length, index)" << std::endl;
	for(int i = 0; i < parsing_bwd.size(); ++i)
		std::cout << std::get<0>(parsing_bwd[i]) << " " << std::get<1>(parsing_bwd[i]) << " " << std::get<2>(parsing_bwd[i]) << std::endl;
	std::vector<size_t> match_pos_fwd;
	std::vector<size_t> match_pos_bwd;
	svs(parsing_fwd, index, match_pos_fwd, fwd_parse_start);
	svs(parsing_bwd, index, match_pos_bwd, bwd_parse_start);
	std::cout << "svs fwd output - " << std::endl;
	for(int i = 0; i < match_pos_fwd.size(); ++i)
		std::cout << match_pos_fwd[i] << " ";
	std::cout << std::endl;
	std::cout << "svs bwd output - " << std::endl;
	for(int i = 0; i < match_pos_bwd.size(); ++i)
		std::cout << match_pos_bwd[i] << " ";
	std::cout << std::endl;

	return 0;
}
