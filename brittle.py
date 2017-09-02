# I give up on C++

import os
import sys
import hashlib
import base64

def do_xor(a, b):
	res = []
	for i in range(len(a)):
		res.append(bytes([a[i] ^ b[i % len(b)]]))
	return b"".join(res)

def encode(dat):
	# Get hash of file
	m = hashlib.sha1()
	m.update(dat)
	hash_a = m.digest()
	
	# XOR data with hash A
	dat = do_xor(dat, hash_a)
	
	# Get hash B
	m = hashlib.sha1()
	m.update(dat)
	hash_b = m.digest()
	
	# Get key
	key = do_xor(hash_a, hash_b)
	
	return key + dat

def decode(dat):
	# Get hash of file
	m = hashlib.sha1()
	m.update(dat[20:])
	hash_b = m.digest()
	
	# Deduce hash A from hash B and key
	hash_a = do_xor(hash_b, dat[:20])
	
	# Recover file
	dat = do_xor(dat, hash_a)[20:]
	
	# Check file integrity
	m = hashlib.sha1()
	m.update(dat)
	if m.digest() == hash_a:
		print("File decoded successfully")
	else:
		print("File does not match hash")
	
	return dat

def main():
	# Open file in binary mode
	with open(sys.argv[1], "rb") as f:
		inp = f.read()
		
		if len(sys.argv) > 3:
			res = decode(inp)
		else:
			res = encode(inp)
		
		# Save output file
		with open(sys.argv[2], "wb") as o:
			o.write(res)
	
if __name__ == "__main__":
	main()
