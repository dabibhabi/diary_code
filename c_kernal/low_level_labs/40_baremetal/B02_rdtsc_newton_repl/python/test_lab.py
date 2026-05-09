"""B02 oracle: REPL command surface + sqrt correctness."""

# TODO: implement.
#
# import os, math
# import pytest
# from .bindings import connect, send
#
# COM1 = "/tmp/com1"
#
# @pytest.fixture
# def repl():
#     if not os.path.exists(COM1):
#         pytest.skip("QEMU not running")
#     yield connect()
#
# def test_help(repl):
#     out = send(repl, "help")
#     for cmd in ("cycles", "sqrt", "help", "quit"):
#         assert cmd in out
#
# @pytest.mark.parametrize("x", [1, 4, 100, 1_000_000])
# def test_sqrt(repl, x):
#     out = send(repl, f"sqrt {x}")
#     # parse the Q32.32 result printed somewhere in `out`
#     # convert to float, compare against math.sqrt
#     ...
