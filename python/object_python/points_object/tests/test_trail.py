from src import Point, Trail


def test_trail_starts_at_start():
    t = Trail(Point(0, 0))
    assert t.coords == (0, 0)
    assert len(t.history) == 1


def test_step_updates_position():
    t = Trail(Point(0, 0))
    t.step(3, 4)
    assert t.coords == (3, 4)


def test_step_appends_to_history():
    t = Trail(Point(0, 0))
    t.step(1, 1)
    t.step(2, 2)
    assert [p.coords for p in t.history] == [(0, 0), (1, 1), (3, 3)]


def test_trail_works_with_distance():
    t = Trail(Point(0, 0))
    t.step(3, 4)
    assert Point(0, 0).distance_to(t) == 5.0


def test_trail_is_any_dimension():
    t = Trail(Point(1, 1, 1, 1))
    t.step(1, 1, 1, 1)
    assert t.coords == (2, 2, 2, 2)
