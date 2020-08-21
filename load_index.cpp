#include<unordered_map>
#include<vector>
#include<fstream>
#include<string>
#include<iostream>

int main()
{
	std::unordered_map<std::string, std::vector<size_t>> index;
	std::ifstream ifs("ecoli_k_grams_dna_8_index.txt");
	std::string buffer;
	size_t list_size = 0;
	size_t list_val = 0;
	while(ifs)
	{
		ifs >> buffer;
//		std::cout << buffer << " ";
		index.insert(std::make_pair(buffer, std::vector<size_t>()));
		ifs >> list_size;
//		std::cout << list_size << std::endl;
		for(int i = 0; i < list_size; ++i)
		{
			ifs >> list_val;
	       		index.at(buffer).push_back(list_val);	
		}
//		ifs >> buffer;
	}
	return 0;
}



