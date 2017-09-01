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
#include <vector>
#include <iterator>
#include "sha1.cpp"

// Generate SHA-1 digest
void sha1_digest(char *in, size_t len, uint8_t out[SHA1_DIGEST_SIZE]) {
	SHA1_CTX context;
	
	SHA1_Init(&context);
	SHA1_Update(&context, (uint8_t*) in, len);
	//SHA1_Final(&context, out);
	for(int i = 0; i < SHA1_DIGEST_SIZE; i++) {
		out[i] = 0;
	}
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

// XOR a char vector
void xor_chars(char *subject, size_t len, uint8_t key[SHA1_DIGEST_SIZE]) {
	for(int i = 0; i < len; i += SHA1_DIGEST_SIZE) {
		for(int j = 0; j < SHA1_DIGEST_SIZE; j++) {
			if(i + j < len)
				subject[i+j] = subject[i+j] ^ key[j];
		}
	}
}

// XOR hash digests (tested, working)
void xor_digests(uint8_t a[SHA1_DIGEST_SIZE], uint8_t b[SHA1_DIGEST_SIZE], uint8_t out[SHA1_DIGEST_SIZE]) {
	for(int i = 0; i < SHA1_DIGEST_SIZE; i++) {
		out[i] = a[i] ^ b[i];
	}
}

// Convert uint8_t array to char (tested, maybe working)
char digest_to_string(uint8_t digest[SHA1_DIGEST_SIZE]) {
	char res;
	for(int i = 0; i < SHA1_DIGEST_SIZE; i++) {
  	res += digest[i];
	}
	return res;
}

// Encode file to a brittle format
char encode(char *in, size_t len) {
	// Get hash of entire file
	uint8_t hash_a[SHA1_DIGEST_SIZE];
	sha1_digest(in, len, hash_a);
	
	// XOR file using hash A
	//char *xord = 
	xor_chars(in, len, hash_a);
	
	// Get hash of XOR'd file
	uint8_t hash_b[SHA1_DIGEST_SIZE];
	sha1_digest(in, len, hash_b);
	
	// XOR hash A with hash B
	uint8_t hash_final[SHA1_DIGEST_SIZE];
	xor_digests(hash_a, hash_b, hash_final);
	
	// Piece it together
	//return digest_to_string(hash_final) + xord;
	std::cerr << hash_a;
	return *in;
}

// Verifies integrity of brittle file and decodes it
char decode(char *in, size_t len) {
	
}

int main(int argc, char *argv[]) {
	// Read data from stdin
	//char s(std::istreambuf_iterator<char>(std::cin), {});
	std::vector<char> data(std::istream_iterator<char>{std::cin}, std::istream_iterator<char>{});
	size_t len = data.size();
	char dat[len+1];
	
	for(int i = 0; i < data.size(); i++)
		dat[i] = data[i];
	
	if(argc > 1) {
		// Assume user wants to decode
		std::cout << decode(dat, len);
	} else {
		// No arguments, encode data
		std::cout << encode(dat, len);
	}
}

/*
TODO
- replace std::string with char arrays
- try XOR-ing with 1's
- check if sha1 is correct
- debug string xor
- check xor'd hash correct
*/
