#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<random>
#include<unordered_map>
#include<fstream>
#include"utils.h"

int main(int argc, char** argv)
{
	if(argc < 4)
	{
		std::cerr << "usage - " << argv[0] << "\tpath to codebook" << "\tpath to sequence to index" << "\toutput file" << std::endl; 
		return -1;
	}
	std::string codebook_file = argv[1];
	std::string ref_seq_file = argv[2];
	std::string out_file = argv[3];
	
	std::vector<char> alphabet; 
	std::vector<std::string> codebook;
	std::string ref_seq;
	std::unordered_map<std::string, std::vector<size_t>> index;

	load_codebook(codebook_file, codebook, alphabet);
	load_ref_seq(ref_seq_file, ref_seq);
	std::cout << "reference sequence length = " << ref_seq.size() << std::endl;
	build_index(ref_seq, codebook, index);
	write_index(out_file, index);
	return 0;
}
	       	
       	
