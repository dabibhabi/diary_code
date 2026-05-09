"""Host harness for B01.

Connects to QEMU's COM1 over a Unix socket, reads 16 lines of hex, parses to
uint64s. Cross-checks against userspace C and pure-Python references.

Run sequence:
  Terminal A: make run        # starts QEMU, blocks
  Terminal B: make harness    # this script
"""

# TODO: implement.
#
# import socket
# import numpy as np
# from pathlib import Path
#
# COM1 = "/tmp/com1"
#
# def read_kernel(n_lines: int = 16, com1: str = COM1) -> list[int]:
#     s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
#     s.connect(com1)
#     buf = b""
#     while buf.count(b"\n") < n_lines:
#         buf += s.recv(4096)
#     s.close()
#     return [int(line, 16) for line in buf.decode().splitlines()[:n_lines]]
#
# def pyxorshift(n: int, seed: int = 1) -> list[int]:
#     state = seed
#     out = []
#     for _ in range(n):
#         state ^= (state << 13) & 0xFFFFFFFFFFFFFFFF
#         state ^= (state >> 7)  & 0xFFFFFFFFFFFFFFFF
#         state ^= (state << 17) & 0xFFFFFFFFFFFFFFFF
#         out.append(state)
#     return out
#
# if __name__ == "__main__":
#     k = read_kernel()
#     p = pyxorshift(16)
#     print("kernel:", [f"{x:016x}" for x in k])
#     print("python:", [f"{x:016x}" for x in p])
#     assert k == p, "MISMATCH"
#     print("OK")
