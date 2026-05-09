# K05 — saxpy in kernel via netlink (a deliberate anti-example)

## Goal
Build a netlink-based RPC: userspace asks the kernel to compute `y = α·x + y` on an array. Kernel does the work and replies. Then **measure how much slower this is than just doing saxpy in userspace**, and write a paragraph explaining why this design is wrong.

## Why this is interesting
The kernel is for things userspace can't or shouldn't do. Numerical computation is emphatically not one of those things. This lab teaches netlink (a real, useful kernel ↔ userspace IPC mechanism — used by `iproute2`, audit, taskstats, ethtool) by building something you'd never deploy.

You'll see:
- Netlink socket setup (`NETLINK_USERSOCK` or a custom protocol number)
- Generic netlink (`genetlink`) for nicer command/attribute model
- Why crossing the user/kernel boundary for compute is a 100× slowdown
- The `copy_from_user` / `copy_to_user` cost is the real story

## Prereqs
- K01–K04 done (you're comfortable with module mechanics)
- Read `Documentation/userspace-api/netlink/intro.rst`

## What to do
1. Define a generic netlink family `nm_compute` with one command `NM_CMD_SAXPY` taking attributes `ALPHA` (f64), `X` (binary blob of f64s), `Y` (binary blob of f64s).
2. In the kernel handler:
   - Receive the message
   - Allocate buffers, `copy_from_user` (or use the netlink-provided buffer)
   - Wrap with `kernel_fpu_begin/end`
   - Call the in-kernel saxpy port (reuse `cpp/numerical_methods/c/blas1.c` algorithm)
   - Build a reply with the new Y
3. Userspace client (Python via `pyroute2`):
   - Sends ALPHA, X, Y
   - Gets back new Y
   - Compares against numpy's `y += alpha * x`
4. Benchmark: same operation done purely in userspace. Compare wall-clock.

## Measurable outcome
- Correctness: kernel result == numpy result (bit-exact for the same algorithm)
- Userspace path is 50–500× faster than the netlink path (depending on N)
- Writeup explains why: copy_from_user, scheduling delay, generic netlink message-parsing overhead

## Reuse
- `cpp/numerical_methods/c/blas1.c` — the algorithm you're (mis)porting

## Hint at next track
That's it for K-track. Track B awaits — you'll do similar work but on your own bare-metal kernel under `c_kernal/test_project/`, with no libc and no FPU.

## References
See `references.md`.
