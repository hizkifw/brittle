#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#define SHA1_DIGEST_SIZE 20

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

// XOR hash digests
void xor_digests(uint8_t a[SHA1_DIGEST_SIZE], uint8_t b[SHA1_DIGEST_SIZE], uint8_t out[SHA1_DIGEST_SIZE]) {
	for(int i = 0; i < SHA1_DIGEST_SIZE; i++) {
		out[i] = a[i] ^ b[i];
	}
}

// Convert uint8_t array to std::string
std::string digest_to_string(uint8_t digest[SHA1_DIGEST_SIZE]) {
	std::string res;
	for(int i = 0; i < SHA1_DIGEST_SIZE; i++) {
  	res += digest[i];
	}
	return res;
}

int main() {
  uint8_t j[20] = {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};
  uint8_t k[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::cout << xor_string("Hello, world!", k) << std::endl;
  
  uint8_t l[20];
  xor_digests(j, k, l);
  std::cout << l << std::endl;
  
  std::cout << digest_to_string(l);
}