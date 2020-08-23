# FB-SvS-Cpp
A multi-threaded C++ 11 implementation of the FB-SvS algorithm for indexed, exact string matching on genomic data

To generate a kmer codebook, compile and run gen_k_grams_code.cpp - 
```
g++ -std=c++0x -gen_k_grams_code.cpp -o gen_k_grams_code.out
./gen_k_grams.out ACGT 8 /path/to/codebook.txt
```
This creates a codebook with all 8-mers with the alphabet = {A,C,G,T}

To build an index on a sequence to index using a codebook file, compile and run build_index_codebook.cpp -
```
g++ -std=c++0x -build_index_codebook.cpp -o build_index_codebook.out
./build_index_codebook.out /path/to/codebook.txt /path/to/ref_seq.txt /path/to/index.txt
```

To test the speed of querying using the synthetic data, compile and run speed_test_synth.cpp -
```
g++ -std=c++0x -fopenmp -O2 -speed_test_synth.cpp -o speed_test_synth.out
./speed_test_synth.out 8 ACGT 1000000 10000 100 500
```
This randomly generates a reference sequence of length 1000000 over the alphabet {A,C,G,T}, with a uniform distribution over the alphabet. The codebook contains all 8-mers. The number of queries is 10000 and their lengths range between 100 and 500 randomly with a uniform distribution

To test the speed of querying using real data in the form of a .fasta file, compile and run speed_test_fb_svs_fastq_queries.cpp -
```
g++ -std=c++0x -fopenmp -O2 -speed_test_fb_svs_fastq_queries.cpp -o speed_test_fb_svs_fastq_queries.out
./speed_test_fb_svs_fastq_queries.out /path/to/codebook.txt /path/to/index.txt /path/to/queries.fastq
```

M. Suri and S. Rini, "Complete Fix-Free Codes for the Statistical Dictionary-Based String Matching Problem," 2019 53rd Asilomar Conference on Signals, Systems, and Computers, Pacific Grove, CA, USA, 2019, pp. 1389-1393, doi: 10.1109/IEEECONF44664.2019.9048814.
https://ieeexplore.ieee.org/document/9048814
