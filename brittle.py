"""
brittle.py

Force file integrity by "encrypting" the file with itself, in a way.
"""

# Stop pylint from complaining about unnecessary parentheses
from __future__ import print_function
import sys
import hashlib


def do_xor(subject, key):
    """
    XORs two strings
    """
    res = []

    # Go through each byte in subject
    for i in enumerate(subject):
        # Append XOR'd byte to results array
        res.append(bytes([i[1] ^ key[i[0] % len(key)]]))

    # Join array to a binary string
    return b"".join(res)


def encode(dat):
    """
    Encode file to brittle format
    """
    # Get hash of file
    digest = hashlib.sha1()
    digest.update(dat)
    hash_a = digest.digest()

    # XOR data with hash A
    dat = do_xor(dat, hash_a)

    # Get hash B
    digest = hashlib.sha1()
    digest.update(dat)
    hash_b = digest.digest()

    # Get key
    key = do_xor(hash_a, hash_b)

    return key + dat


def decode(dat):
    """
    Decode file from brittle format
    """
    # Check file validity
    if len(dat) < 20:
        raise Exception("Invalid file")

    # Get hash of file
    digest = hashlib.sha1()
    digest.update(dat[20:])
    hash_b = digest.digest()

    # Deduce hash A from hash B and key
    hash_a = do_xor(hash_b, dat[:20])

    # Recover file
    dat = do_xor(dat, hash_a)[20:]

    # Check file integrity
    digest = hashlib.sha1()
    digest.update(dat)
    if digest.digest() == hash_a:
        print("File decoded successfully")
    else:
        print("File does not match hash")
        raise Exception("File corrupted")

    return dat


def usage():
    """
    Print help
    """
    print("Usage: {} infile outfile [--decode]".format(sys.argv[0]))
    print("Options:")
    print("  --decode, -d: decode infile to outfiile.")
    print("If not specified, file will be encoded instead")


def main():
    """
    Main function
    """
    if len(sys.argv) < 2:
        return usage()

    # Open file in binary mode
    with open(sys.argv[1], "rb") as infile:
        inp = infile.read()

        if "-d" in sys.argv or "--decode" in sys.argv:
            res = decode(inp)
        elif len(sys.argv) == 3:
            res = encode(inp)
        else:
            return usage()

        # Save output file
        with open(sys.argv[2], "wb") as outfile:
            outfile.write(res)

    return 0

if __name__ == "__main__":
    main()
