"""ioctl wrappers for the nm_newton char device."""

# TODO: implement once the module exposes ioctls.
#
# import ctypes, fcntl, struct
#
# DEVICE = "/dev/nm_newton"
#
# # ioctl numbers must match the module's headers
# NM_IOC_SQRT_Q32 = 0xC0086E01  # _IOWR('n', 1, u64) — example
# NM_IOC_SQRT_F64 = 0xC0086E02
#
# def kernel_sqrt_q32(x_q32: int) -> int:
#     with open(DEVICE, "rb+", buffering=0) as f:
#         buf = bytearray(struct.pack("Q", x_q32))
#         fcntl.ioctl(f, NM_IOC_SQRT_Q32, buf)
#         return struct.unpack("Q", buf)[0]
#
# def kernel_sqrt_f64(x: float) -> float:
#     with open(DEVICE, "rb+", buffering=0) as f:
#         buf = bytearray(struct.pack("d", x))
#         fcntl.ioctl(f, NM_IOC_SQRT_F64, buf)
#         return struct.unpack("d", buf)[0]
