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
	size_t k = 4;
	const std::vector<char> alphabet = {'A', 'T', 'C', 'G'}; 
	size_t alphabet_size = alphabet.size();
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, alphabet_size - 1);
	size_t ref_seq_len = 1000000;
	std::string ref_seq (ref_seq_len, alphabet[0]);
	for(int i = 0; i < ref_seq_len; ++i)
		ref_seq[i] = alphabet[distribution(generator)];
//	std::cout << "referece sequence -" << std::endl;
//	for(int i = 0; i < ref_seq_len; ++i)
//		std::cout << ref_seq[i];
//	std::cout<<std::endl;
	std::unordered_map<std::string, std::vector<size_t>> index;
	std::vector<std::string> code;
	gen_k_grams_code(k, alphabet, code);
	auto max_code_it = std::max_element(code.begin(), code.end(), [] (std::string x1, std::string x2){ return x1.size() < x2.size();});
	size_t max_code_len = max_code_it->size();
	build_index(ref_seq, code, index);

	size_t query_len = 20;
//	std::string query(query_len, alphabet[0]);
//	for(int i = 0; i < query_len; ++i)
//		query[i] = alphabet[distribution(generator)];
	std::string query = ref_seq.substr(31415, query_len);
	std::cout << "query sequence -" << std::endl;
	std::cout << query << std::endl;
	std::vector<size_t> match_pos;
	int n_matches = fb_svs(query, index, max_code_len, match_pos);
	std::cout << "fb-svs output -  " << std::endl;
	for(int i = 0; i < match_pos.size(); ++i)
		std::cout << match_pos[i] << " ";
	std::cout << std::endl;
	std::cout << "string::find() output - " << std::endl;
	size_t start = 0;
	while(1)
	{
		size_t res = ref_seq.find(query, start);
		if (res != std::string::npos)
		{
			std::cout << res << " ";
			start = res + 1;
		}
		else
			break;
	}
	std::cout << std::endl;

	return 0;
}
