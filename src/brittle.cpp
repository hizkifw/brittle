/*
 * brittle
 * (c) 2017 HizkiFW
 *
 * usage:
 *   cat file.txt | ./brittle > out.bin
 *   cat out.bin | ./brittle -d > file.txt
 */

//#define DIGEST_LENGTH 20

#include<iostream>
#include<string>
#include <sstream>
#include "sha1.cpp"

// Generate SHA-1 digest
void sha1_digest(std::string in, uint8_t out[SHA1_DIGEST_SIZE]) {
	SHA1_CTX context;
	
	SHA1_Init(&context);
	SHA1_Update(&context, (uint8_t*) in.c_str(), in.length());
	SHA1_Final(&context, out);
}

// XOR a string
std::string xor_string(std::string subject, uint8_t key[SHA1_DIGEST_SIZE]) {
	
}

// XOR hash digests
void xor_digests(uint8_t a[SHA1_DIGEST_SIZE], uint8_t b[SHA1_DIGEST_SIZE], uint8_t out[SHA1_DIGEST_SIZE]) {
	for(int i = 0; i < SHA1_DIGEST_SIZE; i++) {
		out[i] = a[i] ^ b[i];
	}
}

// Convert uint8_t array to std::string
std::string digest_to_string(uint8_t digest[SHA1_DIGEST_SIZE]) {
	std::ostringstream convert;
	for(int i = 0; i < SHA1_DIGEST_SIZE; i++) {
		convert << (int) digest[i];
	}

	return convert.str();
}

// Encode file to a brittle format
std::string encode(std::string in) {
	// Get hash of entire file
	uint8_t hash_a[SHA1_DIGEST_SIZE];
	sha1_digest(in, hash_a);
	
	// XOR file using hash A
	std::string xord = xor_string(in, hash_a);
	
	// Get hash of XOR'd file
	uint8_t hash_b[SHA1_DIGEST_SIZE];
	sha1_digest(xord, hash_b);
	
	// XOR hash A with hash B
	uint8_t hash_final[SHA1_DIGEST_SIZE];
	xor_digests(hash_a, hash_b, hash_final);
	
	// Piece it together
	//return 
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
