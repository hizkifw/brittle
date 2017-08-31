/*
 * brittle
 * (c) 2017 HizkiFW
 *
 * usage:
 *   cat file.txt | ./brittle > out.bin
 *   cat out.bin | ./brittle -d > file.txt
 */

#include<iostream>
#include<string>
#include "sha1.cpp"

std::string xor_string(std::string subject, size_t key) {
	
}

// Encode file to a brittle format
std::string encode(std::string in) {
	// Get hash of entire data
	std::size_t hash_a = std::hash<std::string>{}(in);
	
}

// Verifies integrity of brittle file and decodes it
std::string decode(std::string in) {
	
}

int main(int argc, char *argv[]) {
	// Read data from stdin
	std::string s(std::istreambuf_iterator<char>(std::cin), {});
	
	if(argc > 1) {
		// Assume user wants to decode
		std::cout << decode(s);
	} else {
		// No arguments, encode data
		std::cout << encode(s);
	}
}
