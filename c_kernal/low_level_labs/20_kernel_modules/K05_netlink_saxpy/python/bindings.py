"""pyroute2 client for the nm_compute netlink family + numpy bench."""

# TODO: implement.
#
# import time
# import numpy as np
# from pyroute2.netlink import genlmsg
# from pyroute2.netlink.generic import GenericNetlinkSocket
#
# class NMComputeClient(GenericNetlinkSocket):
#     def __init__(self):
#         super().__init__()
#         self.bind("nm_compute", genlmsg)  # family name registered by kernel module
#
#     def saxpy(self, alpha: float, x: np.ndarray, y: np.ndarray) -> np.ndarray:
#         msg = genlmsg()
#         msg['cmd']    = 1  # NM_CMD_SAXPY
#         msg['attrs']  = [('NM_ATTR_ALPHA', alpha),
#                          ('NM_ATTR_X', x.tobytes()),
#                          ('NM_ATTR_Y', y.tobytes())]
#         resp = self.nlm_request(msg, msg_type=self.prid)
#         return np.frombuffer(resp[0].get_attr('NM_ATTR_Y'), dtype=np.float64)
#
# def bench(Ns=(1024, 65536, 1_000_000)):
#     rng = np.random.default_rng(0)
#     client = NMComputeClient()
#     for N in Ns:
#         x = rng.standard_normal(N); y = rng.standard_normal(N)
#         t0 = time.perf_counter(); y_np = y + 0.5 * x; t_np = time.perf_counter() - t0
#         t0 = time.perf_counter(); y_k  = client.saxpy(0.5, x, y); t_k  = time.perf_counter() - t0
#         np.testing.assert_allclose(y_np, y_k, rtol=1e-12)
#         print(f"N={N:>8} numpy={t_np*1e3:7.2f}ms  kernel={t_k*1e3:7.2f}ms  slowdown={t_k/t_np:5.1f}x")
#
# if __name__ == "__main__":
#     bench()
