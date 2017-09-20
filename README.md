# brittle

A format that makes a file completely unreadable if any part of it gets corrupted.

Usage:

- `python ./brittle.py file.txt out.bin`
- `python ./brittle.py file.bin out.txt --decode`

## How it works

### Encoding

1. Get hash of entire file (call it hash A)
2. XOR file using hash A
3. Get hash of XOR'd file (call it hash B)
4. XOR hash A using hash B (call it key)
5. Put key at beginning of file

### Decoding

1. Hash file excluding key (we get hash B)
2. XOR key using hash B (we get hash A)
3. XOR file excluding key with hash A
4. File is good if its hash matches hash A
