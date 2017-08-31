/*
 * brittle
 * (c) 2017 HizkiFW
 *
 * usage:
 *   cat file.txt | ./brittle > out.bin
 *   cat out.bin | ./brittle -d > file.txt
 */

//#define DIGEST_LENGTH 20

#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include "sha1.cpp"

// Generate SHA-1 digest
void sha1_digest(std::string in, uint8_t out[SHA1_DIGEST_SIZE]) {
	SHA1_CTX context;
	
	SHA1_Init(&context);
	SHA1_Update(&context, (uint8_t*) in.c_str(), in.length());
	SHA1_Final(&context, out);
}

// Generate hex digest
void digest_to_hex(const uint8_t digest[SHA1_DIGEST_SIZE], char *output)
{
    int i,j;
    char *c = output;

    for (i = 0; i < SHA1_DIGEST_SIZE/4; i++) {
        for (j = 0; j < 4; j++) {
            sprintf(c,"%02X", digest[i*4+j]);
            c += 2;
        }
        sprintf(c, " ");
        c += 1;
    }
    *(c - 1) = '\0';
}

// XOR a string (tested, working)
std::string xor_string(std::string subject, uint8_t key[SHA1_DIGEST_SIZE]) {
	char *outp = new char[subject.length()+1];
	strncpy(outp, subject.c_str(), subject.length());
	
	for(int i = 0; i < subject.length(); i += SHA1_DIGEST_SIZE) {
		for(int j = 0; j < SHA1_DIGEST_SIZE; j++) {
			if(i + j < subject.length())
				outp[i+j] = outp[i+j] ^ key[j];
		}
	}
	
	return std::string(outp);
}

// XOR hash digests (tested, working)
void xor_digests(uint8_t a[SHA1_DIGEST_SIZE], uint8_t b[SHA1_DIGEST_SIZE], uint8_t out[SHA1_DIGEST_SIZE]) {
	for(int i = 0; i < SHA1_DIGEST_SIZE; i++) {
		out[i] = a[i] ^ b[i];
	}
}

// Convert uint8_t array to std::string (tested, maybe working)
std::string digest_to_string(uint8_t digest[SHA1_DIGEST_SIZE]) {
	std::string res;
	for(int i = 0; i < SHA1_DIGEST_SIZE; i++) {
  	res += digest[i];
	}
	return res;
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
	//return digest_to_string(hash_final) + xord;
	std::cerr << hash_a;
	return xord;
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

/*
TODO
- check if sha1 is correct
- debug string xor
- check xor'd hash correct
*/
