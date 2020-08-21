#include<iostream>
#include<vector>
#include<fstream>
#include<random>
#include<omp.h>
#include"utils.h"

int main()
{
	std::string ref_seq;
	std::vector<std::string> codebook;
	std::vector<std::string> queries;
	std::unordered_map<std::string, std::vector<size_t>> index;
	load_codebook("k_grams_dna_8.txt", codebook);
	std::cout <<"finished loading codebook" << std::endl;
	auto max_code_it = std::max_element(codebook.begin(), codebook.end(), [] (std::string x1, std::string x2){ return x1.size() < x2.size();});
	size_t max_code_len = max_code_it->size();
	load_ref_seq("/mnt/d/ecoli.fasta", ref_seq);
	std::cout << "reference sequence length = " << ref_seq.size() << std::endl;
	load_index("ecoli_k_grams_dna_8_index.txt", index);
	std::cout <<"finished loading index" << std::endl;
	load_queries("./ecoli_reads.fastq", queries);
	std::cout <<"finished loading queries" << std::endl;
	size_t n_queries = queries.size();
	#pragma omp parallel for
	for(int i = 0; i < n_queries; ++i)
	{
		std::string query = queries[i];
		if((i % 10000) == 0)
			std::cout << "testing query " << i + 1 << std::endl;
		std::vector<size_t> fb_svs_match_pos;
		int n_matches = fb_svs(query, index, max_code_len, fb_svs_match_pos);
	}
	return 0;
}

