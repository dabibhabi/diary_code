from src import Point, Trail


def main() -> None:
    a = Point(3, 4)
    b = Point(0, 0)
    print(f"{a.coords} -> {b.coords} = {a.distance_to(b)}")

    c = Point(1, 2, 2)
    d = Point(0, 0, 0)
    print(f"{c.coords} -> {d.coords} = {c.distance_to(d)}")

    t = Trail(Point(0, 0, 0))
    t.step(1, 1, 1)
    t.step(1, 1, 1)
    print(f"trail now at {t.coords}, history: {[p.coords for p in t.history]}")
    print(f"distance from {c.coords} to trail = {c.distance_to(t)}")


if __name__ == "__main__":
    main()
