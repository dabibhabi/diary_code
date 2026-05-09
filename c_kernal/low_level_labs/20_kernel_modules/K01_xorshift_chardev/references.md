# K01 references

- The Linux Kernel Module Programming Guide (`sysprog21/lkmpg`) — chapters 1–6 cover everything you need.
- "Linux Device Drivers, 3rd ed." (LDD3, free at `lwn.net/Kernel/LDD3/`) — chapter 3 ("Char Drivers") is the canonical text.
- `Documentation/driver-api/basics.rst`, `Documentation/admin-guide/devices.txt` — kernel docs on device numbers.
- `man 9 alloc_chrdev_region`, `man 9 cdev_init`, `man 9 copy_to_user`.
- `cpp/numerical_methods/c/xorshift.c` — algorithm to port.
- `samples/` directory in the kernel source — read existing simple drivers for style.
