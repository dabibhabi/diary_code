# K02 writeup

## Host
- Kernel:
- Tracepoint registered as: `nm_xorshift:nm_xorshift_gen`

## Verification
- ☐ `available_events` lists our tracepoint
- ☐ bpftrace per-CPU counts ≈ `dd` total ÷ 8
- ☐ BCC Python reader prints live events

## Cost when unattached
- Disable all observers, run K01 throughput test (e.g., `pv < /dev/nm_xorshift > /dev/null`)
- Throughput unattached: ?? MB/s
- Throughput with bpftrace attached: ?? MB/s
- Δ:

## Why static tracepoints beat printk
- (Your 1–2 sentence answer)

## What surprised me
