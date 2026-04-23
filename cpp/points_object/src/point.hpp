#pragma once

#include <initializer_list>
#include <utility>
#include <vector>

#include "locatable.hpp"

// A plain point in any number of dimensions.
class Point : public Locatable {
public:
    Point(std::initializer_list<double> values) : coords_(values) {}
    explicit Point(std::vector<double> values) : coords_(std::move(values)) {}

    const std::vector<double>& coords() const override { return coords_; }

    bool operator==(const Point& other) const { return coords_ == other.coords_; }
    bool operator!=(const Point& other) const { return !(*this == other); }

private:
    std::vector<double> coords_;
};
