#pragma once

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

// An abstract class says: "here is what all these things have in common."
// Any Locatable must tell us its coordinates. Everything else is free.
class Locatable {
public:
    virtual ~Locatable() = default;

    virtual const std::vector<double>& coords() const = 0;

    std::size_t dimension() const { return coords().size(); }

    double distance_to(const Locatable& other) const {
        if (dimension() != other.dimension())
            throw std::invalid_argument("dimensions must match");
        const auto& a = coords();
        const auto& b = other.coords();
        double sum = 0.0;
        for (std::size_t i = 0; i < a.size(); ++i) {
            double d = a[i] - b[i];
            sum += d * d;
        }
        return std::sqrt(sum);
    }
};
