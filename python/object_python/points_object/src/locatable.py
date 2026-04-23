from abc import ABC, abstractmethod
from math import sqrt


# An abstract class says: "here is what all these things have in common."
# Any Locatable must tell us its coordinates. Everything else is free.
class Locatable(ABC):
    @property
    @abstractmethod
    def coords(self) -> tuple[float, ...]: ...

    @property
    def dimension(self) -> int:
        return len(self.coords)

    def distance_to(self, other: "Locatable") -> float:
        if self.dimension != other.dimension:
            raise ValueError("dimensions must match")
        return sqrt(sum((a - b) ** 2 for a, b in zip(self.coords, other.coords)))
