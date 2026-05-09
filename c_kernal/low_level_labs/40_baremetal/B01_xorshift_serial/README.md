# B01 — xorshift over COM1 from your bare-metal kernel

## Goal
Inside `c_kernal/test_project/`, implement enough of the serial driver and the kernel main loop to generate 16 random `uint64_t`s from `xorshift64` and print them to COM1. Pipe COM1 to a Python harness on the host that reads the bytes and runs a chi-square test for uniformity.

## Why this is interesting
- Your first **freestanding** numerical primitive (no libc, no FPU, no kernel services beyond what you wrote)
- You'll see how much "boring" code (boot, GDT, serial init) is required just to print one number
- Closes the loop with statistics: SciPy chi-square confirms randomness from the host side

## Prereqs
- `c_kernal/test_project/` builds (toolchain in `00_setup/`)
- Read OSDev wiki: "Multiboot", "Serial Ports", "Bare Bones tutorial"

## What to do
1. Implement `c_kernal/test_project/drivers/serial.c`:
   - `serial_init()` — program COM1 (0x3F8), 8N1, 115200 baud
   - `serial_putc(char)` — wait for THR-empty, write to UART data register
   - `serial_write(buf, n)`
   - `serial_write_hex_u64(u64)` — 16-char hex, no libc
2. Port `cpp/numerical_methods/c/xorshift.c` into `c_kernal/test_project/lib/xorshift.c`. Strip out anything that needs libc; wrap in `static inline` where appropriate.
3. In `kernel/kernel.c kmain()`:
   - `serial_init()`
   - 16 iterations: write hex of next xorshift64 + newline
4. Build, boot under QEMU with COM1 piped to a Unix socket: `qemu-system-i386 -kernel kernel.elf -serial unix:/tmp/com1,server,nowait -nographic`
5. Python harness (`python/bindings.py`) opens `/tmp/com1`, reads 16 hex lines, parses to `np.uint64`.
6. Run chi-square / Kolmogorov–Smirnov test for uniformity over `[0, 2^64)`.
7. Compare against the userspace reference (build `cpp/numerical_methods/c/xorshift.c` as a `.so`).

## Measurable outcome
- QEMU boots, COM1 prints 16 hex strings, kernel halts cleanly (or `hlt` loops)
- Python harness reads the 16 numbers
- Chi-square on the byte distribution (split each u64 into 8 bytes → 128 byte values) does not reject uniformity (p > 0.01)
- Three-way oracle: bare-metal output == userspace C reference == Python pyxorshift

## Reuse
- `cpp/numerical_methods/c/xorshift.c` — port verbatim
- `c_kernal/test_project/drivers/serial.c` — TODO scaffold; you'll be filling it in here
- `c_kernal/test_project/include/serial.h` — header is already TODO; finalize the API

## Hint at next lab
B02 builds a real serial REPL where you type `cycles` and the kernel runs RDTSC-bracketed Newton's method.

## References
See `references.md`.
