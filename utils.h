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
	std::string key;
	for(int i = 0; i < ref_seq.size(); ++i)
	{
		int j = i;
		key = "";
		while(j < ref_seq.size()) 
		{
			key.push_back(ref_seq[j]);
//			std::string key = ref_seq.substr(i, j - i + 1);
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

size_t parse_query_fwd(const std::string& query, std::unordered_map<std::string, std::vector<size_t>>& index, size_t max_code_len, std::vector<std::tuple<std::string, size_t, size_t>>& parsing)
{
	size_t start = query.size();
	int i = 0;
	std::string substr;
	while(i < query.size())
	{
		int j = i;
		substr = "";
		while(j < query.size())
		{
			if((j - i + 1) > max_code_len)
				break;
			substr.push_back(query[j]);
//			std::string substr = query.substr(i, j - i + 1);
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
		if((j - i + 1) > max_code_len || j == query.size())
			break;
	}
	std::sort(parsing.begin(), parsing.end(), [] (std::tuple<std::string, size_t, size_t> x1, std::tuple<std::string, size_t, size_t> x2) {return std::get<1>(x1) < std::get<1>(x2);}); 
	return start;
}

size_t parse_query_bwd(std::string query, std::unordered_map<std::string, std::vector<size_t>>& index, size_t max_code_len, std::vector<std::tuple<std::string, size_t, size_t>>& parsing)
{
	size_t start = query.size();
	int i = query.size() - 1;
	std::string substr;
	while(i >= 0)
	{
		int j = i;
		substr = "";
		while(j >= 0)
		{
			if((i - j + 1) > max_code_len)
				break;
			substr.insert(substr.cbegin(), query[j]);
//			substr.push_back(query[j]);
//			std::string substr = query.substr(j, i - j + 1);
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
		if((i - j + 1) > max_code_len || j == -1)
			break;
	}
	std::sort(parsing.begin(), parsing.end(), [] (std::tuple<std::string, size_t, size_t> x1, std::tuple<std::string, size_t, size_t> x2) {return std::get<1>(x1) < std::get<1>(x2);}); 
	return start;
}

void svs(std::vector<std::tuple<std::string, size_t, size_t>>& parsing, std::unordered_map<std::string, std::vector<size_t>>& index, std::vector<size_t>& match_pos, size_t start)
{
	if(parsing.size() == 0)
		return;
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

int fb_svs(std::string& query, std::unordered_map<std::string, std::vector<size_t>>& index, size_t max_code_len, std::vector<size_t>& match_pos) 
{
	std::vector<std::tuple<std::string, size_t, size_t>> parsing_fwd;
	std::vector<std::tuple<std::string, size_t, size_t>> parsing_bwd;
	size_t fwd_parse_start = parse_query_fwd(query, index, max_code_len, parsing_fwd);
//	std::cout << "query forward parse - (code word, posting list length, index)" << std::endl;
//	for(int i = 0; i < parsing_fwd.size(); ++i)
//		std::cout << std::get<0>(parsing_fwd[i]) << " " << std::get<1>(parsing_fwd[i]) << " " << std::get<2>(parsing_fwd[i]) << std::endl;
	size_t bwd_parse_start = parse_query_bwd(query, index, max_code_len, parsing_bwd);
//	std::cout << "query backward parse - (code word, posting list length, index)" << std::endl;
//	for(int i = 0; i < parsing_bwd.size(); ++i)
//		std::cout << std::get<0>(parsing_bwd[i]) << " " << std::get<1>(parsing_bwd[i]) << " " << std::get<2>(parsing_bwd[i]) << std::endl;
	if(parsing_fwd.size() == 0 || parsing_bwd.size() == 0)
	{
//		std::cout << "fb-svs failed due to one or more empty parsings" << std::endl;
		return -1;
	}
	std::vector<size_t> match_pos_fwd;
	std::vector<size_t> match_pos_bwd;
	svs(parsing_fwd, index, match_pos_fwd, fwd_parse_start);
	svs(parsing_bwd, index, match_pos_bwd, bwd_parse_start);
	//std::cout << "svs fwd output - " << std::endl;
	//for(int i = 0; i < match_pos_fwd.size(); ++i)
	//	std::cout << match_pos_fwd[i] << " ";
	//std::cout << std::endl;
	//std::cout << "svs bwd output - " << std::endl;
	//for(int i = 0; i < match_pos_bwd.size(); ++i)
	//	std::cout << match_pos_bwd[i] << " ";
	//std::cout << std::endl;
	size_t fwd_parse_len = 0;
	for(int i = 0; i < parsing_fwd.size(); ++i)
		fwd_parse_len += std::get<0>(parsing_fwd[i]).length();
	if(fwd_parse_start + fwd_parse_len - 1 < bwd_parse_start)
	{
//		std::cout << "fb-svs failed due to no overlap between forward and backward parse" << std::endl;
		return -1;
	}
	std::vector<size_t> found;
	int match_offset = 0;
	int d = bwd_parse_start - fwd_parse_start;
	std::vector<size_t>& other = match_pos_bwd;
	if(match_pos_fwd.size() < match_pos_bwd.size())
		match_pos = match_pos_fwd;
	else
	{
		match_pos = match_pos_bwd;
		other = match_pos_fwd;
		d = fwd_parse_start - bwd_parse_start;
		match_offset = d;
	}
	for(int i = 0; i < match_pos.size(); ++i)
	{
		size_t key = match_pos[i] + d; 
		auto it = std::lower_bound(other.begin(), other.end(), key);
		if(*it == key )
			found.push_back(match_pos[i] + match_offset);
	}
	std::move(found.begin(), found.end(), match_pos.begin());
	match_pos.resize(found.size());
	return match_pos.size();
}
