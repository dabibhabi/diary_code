from .locatable import Locatable
from .point import Point


# A point that remembers where it has been.
# Still a Locatable — it just stores its data differently.
class Trail(Locatable):
    def __init__(self, start: Point):
        self.history: list[Point] = [start]

    @property
    def coords(self) -> tuple[float, ...]:
        return self.history[-1].coords

    def step(self, *deltas: float) -> None:
        moved = tuple(c + d for c, d in zip(self.coords, deltas))
        self.history.append(Point(*moved))
