#include "Dataset.hpp"

#include <algorithm>
#include <utility>

namespace stats {

Dataset::Dataset(std::initializer_list<double> values) : data_(values) {}

Dataset::Dataset(std::vector<double> values) : data_(std::move(values)) {}

void Dataset::add(double value) { data_.push_back(value); }

void Dataset::clear() { data_.clear(); }

std::size_t Dataset::size() const noexcept { return data_.size(); }

bool Dataset::empty() const noexcept { return data_.empty(); }

const std::vector<double>& Dataset::values() const noexcept { return data_; }

std::vector<double> Dataset::sorted() const {
    std::vector<double> copy = data_;
    std::sort(copy.begin(), copy.end());
    return copy;
}

}  // namespace stats
