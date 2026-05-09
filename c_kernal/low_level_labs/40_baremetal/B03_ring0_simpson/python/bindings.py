"""B03 host: scrape Simpson results from COM1, diff vs scipy."""

# TODO: implement.
#
# import socket, re
# from scipy.integrate import quad
#
# COM1 = "/tmp/com1"
#
# def read_kernel_results(com1: str = COM1) -> dict[int, float]:
#     s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
#     s.connect(com1)
#     buf = b""
#     while buf.count(b"\n") < 4:
#         buf += s.recv(4096)
#     out = {}
#     for line in buf.decode().splitlines():
#         m = re.match(r"n=\s*(\d+):\s*([\d.]+)", line)
#         if m:
#             out[int(m.group(1))] = float(m.group(2))
#     return out
#
# def reference() -> float:
#     val, _ = quad(lambda x: x * x, 0, 1)
#     return val   # 1/3
