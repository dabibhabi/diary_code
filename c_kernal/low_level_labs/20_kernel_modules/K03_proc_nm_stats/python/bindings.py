"""Parser for /proc/nm_stats."""

# TODO: implement once K03 is loaded.
#
# from pathlib import Path
# from dataclasses import dataclass
#
# @dataclass
# class CpuStats:
#     cpu: int
#     bytes: int
#     calls: int
#
# def read_proc(path: str = "/proc/nm_stats") -> list[CpuStats]:
#     """Parse the seq_file output. Adapt the format to whatever your module emits.
#
#     Example expected format:
#         cpu  bytes        calls
#         0    1048576      131072
#         1    524288       65536
#     """
#     out = []
#     for line in Path(path).read_text().splitlines()[1:]:
#         cpu, b, c = line.split()
#         out.append(CpuStats(int(cpu), int(b), int(c)))
#     return out
