"""pytest oracle for L05.

The eBPF program runs as root in a separate process, so this test focuses on
parsing logic: given a captured histogram (text or JSON), can we extract a
"dominant order" per N? The test runs offline against fixtures.
"""

# TODO: implement once you have a sample histogram captured.
#
# from io import StringIO
#
# SAMPLE_HIST = """
# alloc order        : count     distribution
#         0 -> 1     : 12       |***                                   |
#         2 -> 3     : 4        |*                                     |
#         8 -> 15    : 1024     |****************************************|
# """
#
# def parse_hist(text: str) -> dict[int, int]:
#     """Return {order_lower: count}. Skeleton only."""
#     ...
#
# def test_parse_hist_finds_dominant_order():
#     hist = parse_hist(SAMPLE_HIST)
#     dominant = max(hist, key=hist.get)
#     assert dominant == 8  # order range "8 -> 15"
