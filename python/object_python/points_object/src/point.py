from .locatable import Locatable


# A plain point in any number of dimensions.
class Point(Locatable):
    def __init__(self, *values: float):
        self._coords = tuple(values)

    @property
    def coords(self) -> tuple[float, ...]:
        return self._coords

    def __eq__(self, other: object) -> bool:
        return isinstance(other, Point) and self._coords == other._coords

    def __hash__(self) -> int:
        return hash(self._coords)

    def __repr__(self) -> str:
        return f"Point{self._coords}"
