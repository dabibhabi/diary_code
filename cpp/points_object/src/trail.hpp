#pragma once

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

#include "locatable.hpp"
#include "point.hpp"

// A point that remembers where it  has been.
// Still a Locatable — it just stores its data differently.
class Trail : public Locatable {
public:
    explicit Trail(Point start) : history_{std::move(start)} {}

    const std::vector<double>& coords() const override {
        return history_.back().coords();
    }

    void step(std::initializer_list<double> deltas) {
        const auto& cur = coords();
        if (deltas.size() != cur.size())
            throw std::invalid_argument("step must match dimension");
        std::vector<double> moved(cur.size());
        std::size_t i = 0;
        for (double d : deltas) {
            moved[i] = cur[i] + d;
            ++i;
        }
        history_.emplace_back(std::move(moved));
    }

    const std::vector<Point>& history() const { return history_; }

private:
    std::vector<Point> history_;
};
