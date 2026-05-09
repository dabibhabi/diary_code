"""pytest for K02 — verify the tracepoint exists in tracefs.

Real correctness is exercised by `make trace-bpftrace` while traffic flows;
this just gates that the module + tracepoint are wired up.
"""

# TODO: enable once tracepoint is registered.
#
# import os, pytest
#
# TP = "/sys/kernel/tracing/events/nm_xorshift/nm_xorshift_gen"
#
# @pytest.mark.skipif(not os.path.isdir(TP), reason="tracepoint not present — load nm_xorshift_tp.ko")
# def test_tracepoint_format_has_expected_fields():
#     with open(os.path.join(TP, "format"), "r") as f:
#         fmt = f.read()
#     for field in ("cpu", "bytes", "state"):
#         assert f"field:" in fmt and field in fmt, f"missing field {field}"
