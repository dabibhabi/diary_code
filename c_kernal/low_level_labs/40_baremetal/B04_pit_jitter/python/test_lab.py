"""B04 oracle: histogram count totals 1024, distribution roughly centered."""

# TODO: implement.
#
# import json, os, statistics
# import pytest
#
# @pytest.fixture(scope="module")
# def hist():
#     if not os.path.exists("histogram.json"):
#         pytest.skip("run `make capture` first")
#     with open("histogram.json") as f:
#         return {int(k): int(v) for k, v in json.load(f).items()}
#
# def test_total_samples(hist):
#     assert sum(hist.values()) == 1024
#
# def test_no_pathological_outliers(hist):
#     # No single bin should hold > 90% of samples (would imply broken timer)
#     mode_count = max(hist.values())
#     assert mode_count < 0.9 * sum(hist.values())
