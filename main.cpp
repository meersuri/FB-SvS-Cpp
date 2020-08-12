#include<iostream>
#include<algorithm>
#include<vector>
#include<fstream>
#include <seqan/seq_io.h>
using namespace seqan;
int main()
{
	CharString seqFileName = "/mnt/d/GRCh38/GRCh38_latest_genomic.fasta";
	CharString id;
	IupacString seq;
	SeqFileIn seqFileIn(toCString(seqFileName));
	size_t idx = 0;
	size_t count_uk = 0;
	while(!atEnd(seqFileIn))
	{
		readRecord(id, seq, seqFileIn);
		size_t uk = 0;
		for(int i = 0; i < length(seq); ++i)
		{
			if(!(seq[i] == 'A' || seq[i] == 'C' || seq[i] == 'T' || seq[i] == 'G'))
				++uk;
		}
		idx += length(seq);
		count_uk += uk;
		std::cout << length(seq) << " "<< uk << std::endl;
	}
	std::cout << idx << std::endl << count_uk;
	return 0;
}

