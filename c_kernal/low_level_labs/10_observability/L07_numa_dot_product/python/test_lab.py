"""pytest oracle for L07.

This lab's test is environmental: confirm that on a multi-socket box the local
config is faster than remote. Skip if only one NUMA node.
"""

# TODO: implement once bindings.py works.
#
# import subprocess
# import pytest
# from .bindings import sweep
#
# def _num_nodes() -> int:
#     out = subprocess.check_output(['numactl', '--hardware'], text=True)
#     return out.count('node ') // 2  # crude
#
# @pytest.mark.skipif(_num_nodes() < 2, reason="single-NUMA-node host")
# def test_local_beats_remote():
#     r = sweep()
#     assert r['local'] > r['remote'], f"expected local > remote, got {r}"
