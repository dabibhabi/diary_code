"""B01 oracle: bare-metal kernel output uniformity + match python ref."""

# TODO: implement.
#
# import os
# import numpy as np
# import pytest
# from scipy.stats import chisquare
# from .bindings import read_kernel, pyxorshift
#
# COM1 = "/tmp/com1"
#
# @pytest.fixture(scope="module")
# def kernel_nums():
#     if not os.path.exists(COM1):
#         pytest.skip("QEMU not running; start `make run` in another shell")
#     return read_kernel(16)
#
# def test_kernel_matches_python_ref(kernel_nums):
#     assert kernel_nums == pyxorshift(16)
#
# def test_byte_uniformity(kernel_nums):
#     # split 16 u64s into 128 bytes; chi-square on the 256-bin histogram
#     b = np.array(kernel_nums, dtype=np.uint64).tobytes()
#     hist, _ = np.histogram(np.frombuffer(b, dtype=np.uint8), bins=256, range=(0, 256))
#     # 16 nums × 8 bytes = 128 samples — too few for a real KS test, but
#     # we at least verify the chi-square doesn't blow up
#     assert hist.sum() == 128
