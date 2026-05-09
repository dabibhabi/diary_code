"""BCC tracepoint reader for K02.

Attaches to tracepoint:nm_xorshift:nm_xorshift_gen, prints structured events.
Run with: sudo python3 bindings.py
"""

# TODO: implement once K02 is loaded.
#
# from bcc import BPF
#
# BPF_TEXT = r"""
# #include <uapi/linux/ptrace.h>
#
# struct ev_t {
#     u32 cpu;
#     u32 bytes;
#     u64 state;
#     u64 ts;
# };
# BPF_PERF_OUTPUT(events);
#
# TRACEPOINT_PROBE(nm_xorshift, nm_xorshift_gen) {
#     struct ev_t e = {};
#     e.cpu   = args->cpu;
#     e.bytes = args->bytes;
#     e.state = args->state;
#     e.ts    = bpf_ktime_get_ns();
#     events.perf_submit(args, &e, sizeof(e));
#     return 0;
# }
# """
#
# b = BPF(text=BPF_TEXT)
#
# def cb(cpu, data, size):
#     e = b['events'].event(data)
#     print(f"[cpu={e.cpu}] bytes={e.bytes} state={e.state:016x}")
#
# b['events'].open_perf_buffer(cb)
# print("Tracing... Ctrl-C to exit.")
# while True:
#     try:
#         b.perf_buffer_poll()
#     except KeyboardInterrupt:
#         break
