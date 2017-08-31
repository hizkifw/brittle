# brittle

A file format that becomes completely unreadable if even a single bit is wrong.

Usage:

- `cat file.txt | ./brittle > out.bin`
- `cat out.bin | ./brittle -d > file.txt`

## What it does to the file

### Encoding

1. Get hash of entire file (call it hash A)
2. XOR file using hash A
3. Get hash of XOR'd file (call it hash B)
4. XOR hash A using hash B (call it final hash)
5. Put final hash at beginning of file

### Decoding

1. Hash file excluding final hash (we get hash B)
2. XOR final hash using hash B (we get hash A)
3. XOR entire file with hash A
4. File is good if its hash matches hash A
