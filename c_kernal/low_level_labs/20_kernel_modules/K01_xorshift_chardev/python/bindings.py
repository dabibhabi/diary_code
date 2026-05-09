"""ctypes wrapper for the userspace xorshift reference + kernel-device reader.

Three callers from `test_lab.py`:
  1. Kernel device:  KernelDevice("/dev/nm_xorshift").read(N)
  2. Userspace lib:  UserspaceLib("./libnm_xorshift.so").read(N, seed=...)
  3. Pure Python:    pyxorshift(N, seed=...)  (sanity reference; should match userspace)
"""

# TODO: implement once the userspace .so is built.
#
# import ctypes
# import os
# from pathlib import Path
#
# class KernelDevice:
#     def __init__(self, path: str = "/dev/nm_xorshift"):
#         self.path = path
#     def read(self, n: int) -> bytes:
#         with open(self.path, 'rb') as f:
#             return f.read(n)
#
# class UserspaceLib:
#     def __init__(self, so_path: str):
#         self._lib = ctypes.CDLL(so_path)
#         self._lib.nm_xorshift_seed.argtypes = [ctypes.c_uint64]
#         self._lib.nm_xorshift_next.argtypes = []
#         self._lib.nm_xorshift_next.restype  = ctypes.c_uint64
#     def read(self, n: int, seed: int = 1) -> bytes:
#         self._lib.nm_xorshift_seed(seed)
#         out = bytearray()
#         while len(out) < n:
#             out += int(self._lib.nm_xorshift_next()).to_bytes(8, 'little')
#         return bytes(out[:n])
#
# def pyxorshift(n: int, seed: int = 1) -> bytes:
#     """Reference implementation in pure Python — should match the C version exactly."""
#     state = seed
#     out = bytearray()
#     while len(out) < n:
#         state ^= (state << 13) & 0xFFFFFFFFFFFFFFFF
#         state ^= (state >> 7)  & 0xFFFFFFFFFFFFFFFF
#         state ^= (state << 17) & 0xFFFFFFFFFFFFFFFF
#         out += state.to_bytes(8, 'little')
#     return bytes(out[:n])
