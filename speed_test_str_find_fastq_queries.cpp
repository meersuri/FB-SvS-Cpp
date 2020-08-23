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
	if(argc < 3)
	{
		std::cerr << "usage - " << argv[0] << "  " << "path to sequence" << "  " << "path to queries" << std::endl; 
		return -1;
	}
	std::string ref_seq_file = argv[1];
	std::string queries_file = argv[2];
	
	std::string ref_seq;
	std::vector<std::string> queries;

	load_ref_seq(ref_seq_file, ref_seq);
	std::cout <<"finished loading ref sequence" << std::endl;
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
	}
	steady_clock::time_point tp2 = steady_clock::now();
	milliseconds delta = duration_cast<milliseconds>(tp2 - tp1);
	std::cout << "query time = " << delta.count() << " ms" << std::endl;
	return 0;
}

