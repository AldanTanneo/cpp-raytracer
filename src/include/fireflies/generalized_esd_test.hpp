#ifndef GENERALIZED_ESD_TEST_HPP
#define GENERALIZED_ESD_TEST_HPP

#include <vector>

struct IndexedValue {
    double value;
    size_t index;

    constexpr IndexedValue(double value, size_t index) noexcept
        : value(value), index(index) {}

    constexpr bool operator<(const IndexedValue & other) const noexcept {
        return value < other.value;
    }
};

std::vector<int> generate_outlier_votes(const std::vector<double> & var,
                                        size_t width,
                                        size_t height) noexcept;

#endif
