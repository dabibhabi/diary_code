# K05 writeup

## Host
- Kernel:

## Wall-clock for saxpy(N)
| N | userspace numpy (ms) | kernel via netlink (ms) | slowdown |
|---|---|---|---|
| 1024 |  |  |  |
| 65536 |  |  |  |
| 1e6 |  |  |  |

## Where does the time go in the netlink path?
- Marshalling/copy-to-kernel:
- Scheduling round-trip:
- Actual compute:
- Marshalling back:

## Why this design is wrong
- (Your paragraph)

## Where does this design pattern actually make sense?
- (E.g., what kind of work *would* justify a netlink RPC into the kernel?)

## What surprised me
