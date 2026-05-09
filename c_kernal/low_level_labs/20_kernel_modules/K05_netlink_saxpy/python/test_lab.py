"""pytest oracle: kernel netlink saxpy must match numpy bit-exactly."""

# TODO: implement.
#
# import os
# import numpy as np
# import pytest
# from .bindings import NMComputeClient
#
# @pytest.fixture
# def client():
#     try:
#         return NMComputeClient()
#     except Exception:
#         pytest.skip("nm_compute generic netlink family not registered")
#
# def test_kernel_saxpy_matches_numpy(client):
#     rng = np.random.default_rng(42)
#     x = rng.standard_normal(1024); y = rng.standard_normal(1024)
#     ref = y + 0.5 * x
#     out = client.saxpy(0.5, x, y)
#     np.testing.assert_allclose(out, ref, rtol=1e-12)
