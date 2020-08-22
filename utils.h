#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<unordered_map>
#include<unordered_set>
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

void build_index(std::string& ref_seq, std::vector<std::string>& code, std::unordered_map<std::string, std::unordered_set<size_t>>& index)
{
	for(int i = 0; i < code.size(); ++i)
		index[code[i]] = std::unordered_set<size_t>();
	std::string key;
	for(int i = 0; i < ref_seq.size(); ++i)
	{
		int j = i;
		key = "";
		while(j < ref_seq.size()) 
		{
			key.push_back(ref_seq[j]);
			auto it = index.find(key);
			if(it != index.end())
			{
				it->second.insert(i);
				break;
			}
			++j;
		}
	}
}

size_t parse_query_fwd_k_grams(const std::string& query, std::unordered_map<std::string, std::vector<size_t>>& index, size_t k, std::vector<std::tuple<std::string, size_t, size_t>>& parsing)
{
	size_t start = query.size();
	std::string substr;
	if(query.size() >= k)
	{
		substr = query.substr(0, k);
		parsing.push_back(std::make_tuple(substr, index[substr].size(), 0));
		start = 0;
	}
	for(int i = k; i <= query.size() - k; i += k)
	{
		substr = query.substr(i, k);
		parsing.push_back(std::make_tuple(substr, index[substr].size(), i));
	}
	std::sort(parsing.begin(), parsing.end(), [] (std::tuple<std::string, size_t, size_t> x1, std::tuple<std::string, size_t, size_t> x2) {return std::get<1>(x1) < std::get<1>(x2);}); 
	return start;
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

size_t parse_query_fwd(const std::string& query, std::unordered_map<std::string, std::unordered_set<size_t>>& index, size_t max_code_len, std::vector<std::tuple<std::string, size_t, size_t>>& parsing)
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

size_t parse_query_bwd_k_grams(std::string query, std::unordered_map<std::string, std::vector<size_t>>& index, size_t k, std::vector<std::tuple<std::string, size_t, size_t>>& parsing)
{
	size_t start = query.size();
	std::string substr;
	if(query.size() >= k)
	{
		substr = query.substr(query.size() - k, k);
		parsing.push_back(std::make_tuple(substr, index[substr].size(), query.size() - k));
		start = query.size() - k;
	}
	for(int i = query.size() - 2*k; i >= 0; i -= k)
	{
		substr = query.substr(i, k);
		parsing.push_back(std::make_tuple(substr, index[substr].size(), i));
		start = i;
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

size_t parse_query_bwd(std::string query, std::unordered_map<std::string, std::unordered_set<size_t>>& index, size_t max_code_len, std::vector<std::tuple<std::string, size_t, size_t>>& parsing)
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
	std::vector<size_t> found;
	for(int i = 1; i < parsing.size(); ++i)
	{
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
		found.clear();
	}
	for(int i = 0; i < match_pos.size(); ++i)
		match_pos[i] -= std::get<2>(parsing[0]) - start;
}

void svs(std::vector<std::tuple<std::string, size_t, size_t>>& parsing, std::unordered_map<std::string, std::unordered_set<size_t>>& index, std::unordered_set<size_t>& match_pos, size_t start)
{
	if(parsing.size() == 0)
		return;

	match_pos = index.at(std::get<0>(parsing[0]));
	for(int i = 1; i < parsing.size(); ++i)
	{
		int d = std::get<2>(parsing[i]) - std::get<2>(parsing[0]);
		auto& posting_set = index.at(std::get<0>(parsing[i]));
		auto j = match_pos.begin();
		while(j != match_pos.end())
		{
			size_t key = *j + d;
			auto it = posting_set.find(key);
			if(it == posting_set.end())
				j = match_pos.erase(j);
			else
				++j;
		}
	}
	std::unordered_set<size_t> match_pos_final;
	for(auto it = match_pos.begin(); it != match_pos.end(); ++it)
		match_pos_final.insert(*it - (std::get<2>(parsing[0]) - start));
	match_pos = match_pos_final;
}

int fb_svs_k_grams(std::string& query, std::unordered_map<std::string, std::vector<size_t>>& index, size_t max_code_len, std::vector<size_t>& match_pos) 
{
	std::vector<std::tuple<std::string, size_t, size_t>> parsing_fwd;
	std::vector<std::tuple<std::string, size_t, size_t>> parsing_bwd;
	size_t fwd_parse_start = parse_query_fwd_k_grams(query, index, max_code_len, parsing_fwd);
//	std::cout << "query forward parse - (code word, posting list length, index)" << std::endl;
//	for(int i = 0; i < parsing_fwd.size(); ++i)
//		std::cout << std::get<0>(parsing_fwd[i]) << " " << std::get<1>(parsing_fwd[i]) << " " << std::get<2>(parsing_fwd[i]) << std::endl;
	size_t bwd_parse_start = parse_query_bwd_k_grams(query, index, max_code_len, parsing_bwd);
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
		std::cout << "fb-svs failed due to no overlap between forward and backward parse" << std::endl;
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
		std::cout << "fb-svs failed due to no overlap between forward and backward parse" << std::endl;
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

int fb_svs(std::string& query, std::unordered_map<std::string, std::unordered_set<size_t>>& index, size_t max_code_len, std::unordered_set<size_t>& match_pos) 
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
	std::unordered_set<size_t> match_pos_fwd;
	std::unordered_set<size_t> match_pos_bwd;
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
	int match_offset = 0;
	int d = bwd_parse_start - fwd_parse_start;
	std::unordered_set<size_t>& other = match_pos_bwd;
	if(match_pos_fwd.size() < match_pos_bwd.size())
		match_pos = match_pos_fwd;
	else
	{
		match_pos = match_pos_bwd;
		other = match_pos_fwd;
		d = fwd_parse_start - bwd_parse_start;
		match_offset = d;
	}
	auto it = match_pos.begin();
	while(it != match_pos.end())
	{
		size_t key = *it + d; 
		auto j = other.find(key);
		if(j == other.end())
			it = match_pos.erase(it);
		else
			++it;
	}
	std::unordered_set<size_t> match_pos_final;
	for(auto it = match_pos.begin(); it != match_pos.end(); ++it)
		match_pos_final.insert(*it + match_offset);
	match_pos = match_pos_final;
	return match_pos.size();
}

void load_index(std::string file, std::unordered_map<std::string, std::vector<size_t>>& index)
{
	std::ifstream ifs(file);
	std::string buffer;
	size_t list_size = 0;
	size_t list_val = 0;
	while(ifs)
	{
		ifs >> buffer;
		index.insert(std::make_pair(buffer, std::vector<size_t>()));
		ifs >> list_size;
		for(int i = 0; i < list_size; ++i)
		{
			ifs >> list_val;
	       		index.at(buffer).push_back(list_val);	
		}
	}
	return;
}

void load_queries(std::string file, std::vector<std::string>& queries)
{
	std::ifstream ifs(file);
	char line[500];
	bool start_seq = false;
	while(ifs.getline(line, 500))
	{
		if(start_seq)
		{
			queries.push_back(std::string(line));
			start_seq = false;
		}
		if(line[0] == '@')
			start_seq = true;

	}
}

void load_codebook(std::string file, std::vector<std::string>& codebook, std::vector<char>& alphabet)
{
	std::ifstream ifs(file);
	size_t alphabet_size;
	char symbol;
	ifs >> alphabet_size;
	for(int i = 0; i < alphabet_size; ++i)
	{
		ifs >> symbol;
		alphabet.push_back(symbol);
	}
	std::string codeword;
	while(ifs)
	{
		ifs >> codeword;
		codebook.push_back(codeword);
	}
}

void load_ref_seq(std::string file, std::string& ref_seq)
{
	std::ifstream ifs(file);
	char buffer[100];
	while(ifs.getline(buffer, 100))
	{
		if(buffer[0] != '>')
			ref_seq += std::string(buffer);
	}
}

void write_index(std::string file, std::unordered_map<std::string, std::vector<size_t>>& index)
{
	std::ofstream ofs(file);
	for(auto it = index.begin(); it != index.end(); ++it)
	{
		ofs << it->first << std::endl;
		size_t posting_list_len = it->second.size();
		ofs << std::to_string(posting_list_len) << std::endl;
		for(int i = 0; i < posting_list_len; ++i)
			ofs << std::to_string(it->second[i]) << " ";
		ofs << std::endl;
	}
}

void write_codebook(std::string file, std::vector<std::string>& codebook, std::vector<char>& alphabet)
{
	std::ofstream ofs(file);
	ofs << alphabet.size() << std::endl;
	for(int i = 0; i < alphabet.size(); ++i)
		ofs << alphabet[i] << " ";
	ofs << std::endl;
	for(int i =0; i < codebook.size(); ++i)
		ofs << codebook[i] << std::endl;
}
