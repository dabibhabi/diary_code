"""BCC Python tracer for L05.

Attaches kprobe to __alloc_pages and histograms the order argument while
filtering by comm == "cholesky_workload".

Run with: sudo python3 bindings.py

Note: __alloc_pages was renamed to __alloc_pages_noprof on some recent kernels.
Adjust the kprobe target if attach fails.
"""

# TODO: implement.
#
# from bcc import BPF
#
# BPF_TEXT = r"""
# #include <uapi/linux/ptrace.h>
#
# BPF_HISTOGRAM(orders);
#
# int kprobe____alloc_pages(struct pt_regs *ctx, int order /* arg layout varies by kernel */) {
#     char want[] = "cholesky_workload";
#     char comm[16];
#     bpf_get_current_comm(&comm, sizeof(comm));
#     for (int i = 0; i < sizeof(want); i++) {
#         if (comm[i] != want[i]) return 0;
#     }
#     orders.increment(bpf_log2l(order + 1));
#     return 0;
# }
# """
#
# b = BPF(text=BPF_TEXT)
# # Try both symbol names. Recent kernels may rename to __alloc_pages_noprof.
# try:
#     b.attach_kprobe(event="__alloc_pages", fn_name="kprobe____alloc_pages")
# except Exception:
#     b.attach_kprobe(event="__alloc_pages_noprof", fn_name="kprobe____alloc_pages")
#
# print("Tracing... Ctrl-C to dump histogram.")
# try:
#     while True:
#         pass
# except KeyboardInterrupt:
#     b["orders"].print_log2_hist("alloc order")
