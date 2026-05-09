# 00_setup — host configuration for `low_level_labs`

One-time setup. Run these commands once, then your host is ready for any lab. Re-check after a kernel upgrade.

## Toolchain install

### Debian/Ubuntu
```bash
sudo apt update
sudo apt install -y \
    build-essential clang lld make pkg-config \
    linux-headers-$(uname -r) linux-tools-common linux-tools-$(uname -r) linux-tools-generic \
    bpftrace bpfcc-tools libbpf-dev libbpfcc-dev \
    qemu-system-x86 grub-pc-bin xorriso mtools \
    nasm gdb \
    python3 python3-venv python3-pip
```

### Arch
```bash
sudo pacman -S --needed \
    base-devel clang lld \
    linux-headers perf bpf bcc bcc-tools bpftrace libbpf \
    qemu-system-x86 grub xorriso mtools \
    nasm gdb \
    python python-pip
```

### i686-elf cross-compiler (B-track only)

Build from source per OSDev wiki ([https://wiki.osdev.org/GCC_Cross-Compiler](https://wiki.osdev.org/GCC_Cross-Compiler)) or use a prebuilt toolchain. Confirm with:
```bash
i686-elf-gcc --version
i686-elf-ld --version
```

## Kernel sysctls (relax for learning, restore for shared/prod hosts)

```bash
sudo sysctl kernel.perf_event_paranoid=0   # allow non-root perf
sudo sysctl kernel.kptr_restrict=0          # show kernel pointers (needed for some bpftrace)
sudo sysctl kernel.unprivileged_bpf_disabled=0
```

To make persistent until reboot:
```bash
echo "kernel.perf_event_paranoid = 0" | sudo tee /etc/sysctl.d/99-low-level-labs.conf
echo "kernel.kptr_restrict = 0"        | sudo tee -a /etc/sysctl.d/99-low-level-labs.conf
sudo sysctl --system
```

> **Don't** apply this on a shared workstation, server, or anything reachable from a network you don't control. Learning host only.

## CPU & memory tuning (for stable benchmarks)

```bash
# Performance governor (kills frequency scaling jitter during benchmarks)
sudo cpupower frequency-set -g performance

# Disable turbo boost during measurement (optional but improves repeatability)
echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo

# Reserve 2 MiB hugepages for L05 / L07
echo 512 | sudo tee /proc/sys/vm/nr_hugepages

# Verify
grep Huge /proc/meminfo
```

For NUMA labs (L07): confirm topology with `numactl --hardware` and `lscpu | grep -i numa`. If you only have one NUMA node, L07 still teaches you `numactl` and `perf c2c` but the bandwidth gap will be small.

## Pin a CPU for measurement (optional but recommended)

Add to `/etc/default/grub`'s `GRUB_CMDLINE_LINUX_DEFAULT`:
```
isolcpus=3 nohz_full=3 rcu_nocbs=3
```
Then `sudo update-grub && reboot`. CPU 3 is now isolated; benchmark with `taskset -c 3 ./your_program`.

## Python venv

From the `low_level_labs/` root:
```bash
python -m venv .venv
source .venv/bin/activate
pip install -e .                # uses pyproject.toml
# Optional: BCC Python bindings come from your distro package, not pip.
# Verify import:
python -c "import bcc; print(bcc.__version__)"   # only if you installed bpfcc-tools
```

## Sanity checks

```bash
# perf works without sudo?
perf stat true

# bpftrace works?
sudo bpftrace -l 'tracepoint:syscalls:*' | head

# kernel headers match running kernel?
ls /lib/modules/$(uname -r)/build

# i686-elf cross-compiler present?
i686-elf-gcc -v 2>&1 | grep target

# python venv active?
which python   # should point at .venv/bin/python
```

If all five pass, you're ready. Pick a lab.

## Restoring the host afterwards

```bash
sudo rm /etc/sysctl.d/99-low-level-labs.conf
sudo sysctl --system
sudo cpupower frequency-set -g powersave   # or schedutil
echo 0 | sudo tee /proc/sys/vm/nr_hugepages
echo 0 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
```

Remove `isolcpus` etc. from grub config and re-run `update-grub` if you set them.

## References

See top-level `RESOURCES.md`. Specifically:
- `kernel.org/doc/html/latest/admin-guide/perf-security.rst` — what `perf_event_paranoid` actually controls
- OSDev wiki "GCC Cross-Compiler" — for the i686-elf toolchain
- Brendan Gregg, "Linux Performance" (`brendangregg.com/linuxperf.html`) — methodology
