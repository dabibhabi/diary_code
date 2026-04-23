from src import Point


def test_point_stores_coords():
    assert Point(3, 4).coords == (3, 4)


def test_point_is_any_dimension():
    assert Point(1).dimension == 1
    assert Point(1, 2).dimension == 2
    assert Point(1, 2, 3, 4, 5).dimension == 5


def test_point_equality():
    assert Point(1, 2) == Point(1, 2)
    assert Point(1, 2) != Point(2, 1)


def test_point_is_hashable():
    assert {Point(1, 2), Point(1, 2)} == {Point(1, 2)}
