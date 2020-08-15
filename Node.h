#include<vector>
#include<random>

class Node
{
	public:
		Node();
		Node(char node_val); 
		void extend();
		char val;
		std::vector<Node*> children;
		static std::vector<char> alphabet;
		static std::default_random_engine generator;
		static std::discrete_distribution<int> distribution;
};
