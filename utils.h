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

size_t parse_query_fwd(const std::string& query, std::unordered_map<std::string, std::vector<size_t>>& index, std::vector<std::tuple<std::string, size_t, size_t>>& parsing)
{
	size_t start = query.size();
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
				if(i < start)
					start = i;
				i = j + 1;
				break;
			}
			++j;
		}
		if(j == query.size())
			break;
	}
	sort(parsing.begin(), parsing.end(), [] (std::tuple<std::string, size_t, size_t> x1, std::tuple<std::string, size_t, size_t> x2) {return std::get<1>(x1) < std::get<1>(x2);}); 
	return start;
}

size_t parse_query_bwd(std::string query, std::unordered_map<std::string, std::vector<size_t>>& index, std::vector<std::tuple<std::string, size_t, size_t>>& parsing)
{
	size_t start = query.size();
	int i = query.size() - 1;
	while(i >= 0)
	{
		int j = i;
		while(j >= 0)
		{
			std::string substr = query.substr(j, i - j + 1);
			auto it = index.find(substr);
			if(it != index.end())
			{
				parsing.push_back(std::make_tuple(substr, index[substr].size(), j));
				i = j - 1;
				if(j < start)
					start = j;
				break;
			}
			--j;
		}
		if(j == -1)
			break;
	}
	sort(parsing.begin(), parsing.end(), [] (std::tuple<std::string, size_t, size_t> x1, std::tuple<std::string, size_t, size_t> x2) {return std::get<1>(x1) < std::get<1>(x2);}); 
	return start;
}

void svs(std::vector<std::tuple<std::string, size_t, size_t>>& parsing, std::unordered_map<std::string, std::vector<size_t>>& index, std::vector<size_t>& match_pos, size_t start)
{
	match_pos = index.at(std::get<0>(parsing[0]));
	for(int i = 1; i < parsing.size(); ++i)
	{
		std::vector<size_t> found;
		int d = std::get<2>(parsing[i]) - std::get<2>(parsing[0]);
		for(int j = 0; j < match_pos.size(); ++j)
		{
			std::vector<size_t>& posting_list = index.at(std::get<0>(parsing[i]));
			size_t key = match_pos[j] + d;
			auto it = std::lower_bound(posting_list.begin(), posting_list.end(), key);
			if(*it == key)
				found.push_back(match_pos[j]);
		}
		size_t n = found.size();
		std::move(found.begin(), found.end(), match_pos.begin());
		match_pos.resize(n);
	}
	for(int i = 0; i < match_pos.size(); ++i)
		match_pos[i] -= std::get<2>(parsing[0]) - start;
}
