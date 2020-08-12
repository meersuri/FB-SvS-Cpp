#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<unordered_map>
#include<tuple>

void gen_k_grams_code(size_t k, const std::vector<char>& alphabet, std::vector<std::string>& code)
{
	size_t alphabet_size = alphabet.size();
	for(int i = 0; i < alphabet.size(); ++i)
	{
		std::string code_word(1, alphabet[i]);
		code.push_back(code_word);
	}
	for(int i = 1; i < k; ++i)
	{
		std::vector<std::string> curr_code;
		for(int j = 0; j < code.size(); ++j)
		{
			std::string code_word = code[j];
			for(int m = 0; m < alphabet_size; ++m)
				curr_code.push_back(code_word + alphabet[m]);
		}
		code = std::move(curr_code);
	}
}

void build_index(std::string& ref_seq, std::vector<std::string>& code, std::unordered_map<std::string, std::vector<size_t>>& index)
{
	for(int i = 0; i < code.size(); ++i)
		index[code[i]] = std::vector<size_t>();
	for(int i = 0; i < ref_seq.size(); ++i)
	{
		int j = i;
		while(j < ref_seq.size()) 
		{
			std::string key = ref_seq.substr(i, j - i + 1);
			auto it = index.find(key);
			if(it != index.end())
			{
				it->second.push_back(i);
				break;
			}
			++j;
		}
	}
}

void parse_query_fwd(const std::string& query, std::unordered_map<std::string, std::vector<size_t>>& index, std::vector<std::tuple<std::string, size_t, size_t>>& parsing)
{
	int i = 0;
	while(i < query.size())
	{
		int j = i;
		while(j < query.size())
		{
			std::string substr = query.substr(i, j - i + 1);
			auto it = index.find(substr);
			if(it != index.end())
			{
				parsing.push_back(std::make_tuple(substr, index[substr].size(), i));
				i = j + 1;
				break;
			}
			++j;
		}
		if(j == query.size())
			break;
	}
	sort(parsing.begin(), parsing.end(), [] (std::tuple<std::string, size_t, size_t> x1, std::tuple<std::string, size_t, size_t> x2) {return std::get<1>(x1) < std::get<1>(x2);}); 
}

void parse_query_bwd(std::string query, std::unordered_map<std::string, std::vector<size_t>>& index, std::vector<std::tuple<std::string, size_t, size_t>>& parsing)
{
	std::reverse(query.begin(), query.end());
	int i = 0;
	while(i < query.size())
	{
		int j = i;
		while(j < query.size())
		{
			std::string substr = query.substr(i, j - i + 1);
			auto it = index.find(substr);
			if(it != index.end())
			{
				parsing.push_back(std::make_tuple(substr, index[substr].size(), query.size() - i - 1));
				i = j + 1;
				break;
			}
			++j;
		}
		if(j == query.size())
			break;
	}
	sort(parsing.begin(), parsing.end(), [] (std::tuple<std::string, size_t, size_t> x1, std::tuple<std::string, size_t, size_t> x2) {return std::get<1>(x1) < std::get<1>(x2);}); 
}
