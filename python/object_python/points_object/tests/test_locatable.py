import pytest

from src import Point


def test_distance_2d():
    assert Point(3, 4).distance_to(Point(0, 0)) == 5.0


def test_distance_3d():
    assert Point(1, 2, 2).distance_to(Point(0, 0, 0)) == 3.0


def test_distance_is_symmetric():
    a, b = Point(1, 2, 3), Point(4, 5, 6)
    assert a.distance_to(b) == b.distance_to(a)


def test_distance_to_self_is_zero():
    assert Point(7, 8, 9).distance_to(Point(7, 8, 9)) == 0.0


def test_mismatched_dimensions_raise():
    with pytest.raises(ValueError):
        Point(1, 2).distance_to(Point(1, 2, 3))
