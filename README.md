# brittle

A file format that becomes completely unreadable if even a single bit is wrong.

Usage:

- `cat file.txt | ./brittle > out.bin`
- `cat out.bin | ./brittle -d > file.txt`

## What it does to the file

### Encoding

1. Get hash of entire file (call it hash A)
2. XOR file by hash A
3. Get hash of XOR'd file (call it hash B)
4. XOR hash A using hash B
5. Put XOR'd hash A at beginning of file

### Decoding

1. Hash file excluding hash A (call it hash B)
2. XOR hash A using hash B
3. XOR entire file with XOR'd hash A
4. Verify hash of file matches hash A
