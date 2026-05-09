"""B04 host: read histogram dump from COM1, decode, save JSON."""

# TODO: implement.
#
# import socket, json, re
#
# COM1 = "/tmp/com1"
#
# def read_histogram(com1: str = COM1) -> dict[int, int]:
#     s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
#     s.connect(com1)
#     buf = b""
#     # kernel emits:  HIST_BEGIN\n bin_low,count\n bin_low,count\n ... HIST_END\n
#     while b"HIST_END" not in buf:
#         buf += s.recv(4096)
#     hist = {}
#     for line in buf.decode().splitlines():
#         m = re.match(r"(\d+),(\d+)", line)
#         if m:
#             hist[int(m.group(1))] = int(m.group(2))
#     return hist
#
# if __name__ == "__main__":
#     h = read_histogram()
#     with open("histogram.json", "w") as f:
#         json.dump(h, f)
#     print(f"captured {sum(h.values())} samples across {len(h)} bins → histogram.json")
