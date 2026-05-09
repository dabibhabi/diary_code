"""B02 host harness — interactive REPL + benchmark driver."""

# TODO: implement.
#
# import argparse, socket, sys, time
#
# COM1 = "/tmp/com1"
#
# def connect(path: str = COM1) -> socket.socket:
#     s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
#     s.connect(path)
#     return s
#
# def send(s: socket.socket, line: str) -> str:
#     s.sendall((line + "\n").encode())
#     out = b""
#     while not out.endswith(b"\n> "):
#         out += s.recv(4096)
#     return out.decode()
#
# def interactive():
#     s = connect()
#     while True:
#         line = input("> ")
#         if line == "quit":
#             break
#         print(send(s, line), end="")
#
# def bench(n: int = 100):
#     s = connect()
#     samples = []
#     for _ in range(n):
#         out = send(s, "cycles")
#         # parse a "<number> cyc/op" reply
#         for tok in out.split():
#             try: samples.append(int(tok)); break
#             except ValueError: pass
#     import numpy as np
#     print(f"n={len(samples)} mean={np.mean(samples):.1f} std={np.std(samples):.1f} "
#           f"p99={np.percentile(samples, 99):.1f} min={min(samples)} max={max(samples)}")
#
# if __name__ == "__main__":
#     ap = argparse.ArgumentParser()
#     ap.add_argument("--interactive", action="store_true")
#     ap.add_argument("--bench", action="store_true")
#     args = ap.parse_args()
#     (interactive if args.interactive else bench)()
