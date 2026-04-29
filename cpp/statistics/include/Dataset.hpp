#pragma once

#include <cstddef>
#include <initializer_list>
#include <vector>

namespace stats {

class Dataset {
public:
    Dataset() = default;
    Dataset(std::initializer_list<double> values);
    explicit Dataset(std::vector<double> values);

    void add(double value);
    void clear();

    std::size_t size() const noexcept;
    bool        empty() const noexcept;

    const std::vector<double>& values() const noexcept;
    std::vector<double>        sorted() const;

private:
    std::vector<double> data_;
};

}  // namespace stats
