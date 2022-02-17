#ifndef GENERALIZED_ESD_TEST_HPP
#define GENERALIZED_ESD_TEST_HPP

#include <vector>

// Helper class to store a double along with its index in a buffer
struct IndexedValue {
    double value;
    size_t index;

    constexpr IndexedValue(double value, size_t index) noexcept
        : value(value), index(index) {}

    // Comparison using the stored value
    constexpr bool operator<(const IndexedValue & other) const noexcept {
        return value < other.value;
    }
};

// Generate a vector of outlier votes using a variance buffer
std::vector<int> generate_outlier_votes(const std::vector<double> & var,
                                        size_t width,
                                        size_t height) noexcept;

#endif
