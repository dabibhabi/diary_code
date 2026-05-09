"""pytest for K03."""

# TODO: implement.
#
# import os, subprocess
# import pytest
# from .bindings import read_proc
#
# @pytest.mark.skipif(not os.path.exists("/proc/nm_stats"), reason="K03 module not loaded")
# def test_pinned_cpu_accumulates():
#     before = {s.cpu: s.bytes for s in read_proc()}
#     # Pin some traffic to CPU 0
#     subprocess.run(['taskset', '-c', '0', 'dd', 'if=/dev/nm_xorshift',
#                     'of=/dev/null', 'bs=8', 'count=10000', 'status=none'], check=True)
#     after = {s.cpu: s.bytes for s in read_proc()}
#     # CPU 0 must have grown the most
#     deltas = {cpu: after[cpu] - before[cpu] for cpu in after}
#     assert max(deltas, key=deltas.get) == 0, f"expected CPU 0 to dominate; got {deltas}"
