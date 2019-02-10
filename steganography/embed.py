#!/usr/bin/env python3

import argparse
import re
import sys

def string_to_bits(msg):
    """Convert a string into a sequence of bits"""
    for c in msg.encode():
        for b in range(8):
            yield c & (1 << b) != 0

def int_to_bytes(i):
    """Convert a 32-bit int into a list of byte values in little endian order."""
    assert i < 2**32

    l = []
    for b in range(4):
        l.append((i >> b*8) & 0xFF)

    return l

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Embed messages into object files')
    parser.add_argument('message', help='The message to embed')
    args = parser.parse_args()

    xor_re = re.compile(r'\s*xorl\s+\$(\d+), %eax')

    for bit in string_to_bits(args.message):
        for line in sys.stdin:
            line = line.rstrip()
            m = re.match(xor_re, line)
            if not m:
                print(line)
                continue

            imm_bytes = int_to_bytes(int(m.group(1)))
            short_opc = [0x35]
            long_opc = [0x81, 0xf0]

            instruction = [str(b) for b in (long_opc if bit else short_opc) + imm_bytes]
            print(".byte {}".format(",".join(instruction)))
            break

    # All bits encoded. Just copy remaining lines to output
    for line in sys.stdin:
        print(line.rstrip())
