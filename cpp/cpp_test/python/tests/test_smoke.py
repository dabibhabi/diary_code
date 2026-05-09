"""Smoke test for the Python package scaffolding."""

from __future__ import annotations


def test_package_imports() -> None:
    import cuny_sms

    assert cuny_sms.__version__ == "0.0.1"
