#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<fstream>
#include"utils.h"

int main(int argc, char** argv)
{
	if(argc < 4)
	{
	std::cerr << "usage - " << argv[0] << "  " << "alphabet" << "  " << "codeword length" << "  " << "output file" << std::endl;
		return -1;
	}
	std::vector<char> alphabet;
	std::string alphabet_string = argv[1];
	for(int i = 0; i < alphabet_string.size(); ++i)
		alphabet.push_back(alphabet_string[i]);
	size_t k = std::stoi(argv[2]);
	std::string out_file = argv[3];

	std::vector<std::string> codebook;
	gen_k_grams_code(k, alphabet, codebook);
	std::cout << "codebook size = " << codebook.size() << std::endl;
	write_codebook(out_file, codebook, alphabet);
	return 0;
}

