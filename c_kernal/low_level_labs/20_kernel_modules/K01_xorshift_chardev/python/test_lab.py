"""Three-way oracle for K01.

For pure correctness comparison the kernel module must be **seedable** from
userspace (e.g. via a write() that sets the seed). If you skip that and the
kernel module starts from a fixed compile-time seed, set the userspace ref to
match — the test then asserts byte-equality.
"""

# TODO: implement once K01 is loaded and the userspace .so exists.
#
# import os
# import pytest
# from .bindings import KernelDevice, UserspaceLib, pyxorshift
#
# KERNEL_DEFAULT_SEED = 1   # match whatever your module compiles with
# N = 4096
#
# @pytest.fixture(scope="module")
# def kernel():
#     if not os.path.exists("/dev/nm_xorshift"):
#         pytest.skip("/dev/nm_xorshift not present — load nm_xorshift.ko first")
#     return KernelDevice()
#
# @pytest.fixture(scope="module")
# def user():
#     return UserspaceLib("./libnm_xorshift.so")
#
# def test_three_way_oracle(kernel, user):
#     k = kernel.read(N)
#     u = user.read(N, seed=KERNEL_DEFAULT_SEED)
#     p = pyxorshift(N, seed=KERNEL_DEFAULT_SEED)
#     assert k == u, f"kernel != userspace: first diff at byte {next(i for i,(a,b) in enumerate(zip(k,u)) if a!=b)}"
#     assert u == p, "userspace != python — bug in port"
