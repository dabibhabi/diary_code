"""Python harness for L07.

Driver script: launches numa_dot under each numactl config, parses GB/s,
collects results into a DataFrame for the notebook.
"""

# TODO: implement.
#
# import subprocess, re
# from typing import Iterable
#
# def run(numactl_args: list[str]) -> float:
#     """Run numa_dot under given numactl prefix; return GB/s parsed from stdout."""
#     out = subprocess.check_output(['numactl', *numactl_args, './numa_dot'], text=True)
#     m = re.search(r'GB/s:\s*([\d.]+)', out)
#     assert m, f"could not parse: {out}"
#     return float(m.group(1))
#
# def sweep() -> dict:
#     return {
#         'local':      run(['--membind=0', '--cpunodebind=0']),
#         'remote':     run(['--membind=1', '--cpunodebind=0']),
#         'interleave': run(['--interleave=all', '--cpunodebind=0']),
#     }
