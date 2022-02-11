#include <algorithm>
#include <cmath>
#include <numeric>
#include <queue>
#include <vector>

// From src/include
#include <fireflies/generalized_esd_test.hpp>
#include <fireflies/lambda_values.hpp>

using std::vector;

struct Sums {
    double sum, sum2;

    constexpr Sums(double sum, double sum2) noexcept : sum(sum), sum2(sum2) {}
};

void esd_test(const vector<double> & var,
              const size_t width,
              const size_t height,
              const size_t index,
              vector<int> & votes) noexcept {
    constexpr double MAD_TO_STD_DEV = 0.6745;
    constexpr double THRESHOLD = 3.5;

    vector<IndexedValue> queue;

    const size_t y = index / width;
    const size_t x = index % width;

    const size_t left = std::max((size_t)2, x) - 2;
    const size_t right = std::min(width, x + 3);
    const size_t top = std::max((size_t)2, y) - 2;
    const size_t bottom = std::min(height, y + 3);

    for (size_t j = left; j < right; ++j) {
        for (size_t i = top; i < bottom; ++i) {
            size_t index = i * width + j;
            double variance = var[index];
            queue.push_back(IndexedValue(variance, index));
        }
    }

    const int n = queue.size();
    std::sort(std::begin(queue), std::end(queue));

    double median = queue[n / 2].value;
    if (n % 2 == 0) {
        median += queue[n / 2 + 1].value;
        median *= 0.5;
    }

    int upper_bound = 0;
    {
        // Compute outlier upper bound using the modifed Z score

        vector<double> mad_vec;
        for (const IndexedValue & val : queue) {
            mad_vec.push_back(std::abs(val.value - median));
        }
        std::nth_element(mad_vec.begin(), mad_vec.begin() + n / 2,
                         mad_vec.end());

        double mad = mad_vec[n / 2];
        if (n % 2 == 0) {
            mad +=
                *std::min_element(mad_vec.begin() + n / 2 + 1, mad_vec.end());
            mad *= 0.5;
        }

        for (const double & md : mad_vec) {
            // count all samples with modified Z score above THRESHOLD as
            // possible outliers
            if (MAD_TO_STD_DEV * md / mad > THRESHOLD) {
                upper_bound += 1;
            }
        }
    }

    vector<Sums> sums;
    {
        Sums old_sums(0.0, 0.0);
        for (const IndexedValue & val : queue) {
            old_sums.sum += val.value;
            old_sums.sum2 += val.value * val.value;
            sums.push_back(old_sums);
        }
    }

    const int nprocess = std::min(n, upper_bound);
    vector<double> r_values;
    vector<size_t> outlier_indexes;

    outlier_indexes.reserve(nprocess);
    int count = 0;
    {
        r_values.reserve(nprocess);

        for (int i = 0; i < nprocess; ++i) {
            Sums s = sums.back();
            double mean = s.sum / queue.size();
            double std_dev = std::sqrt(s.sum2 / queue.size() - mean * mean);

            IndexedValue max_val = queue.back();

            r_values.push_back(std::abs(max_val.value - mean) / std_dev);
            outlier_indexes.push_back(max_val.index);

            queue.pop_back();
            sums.pop_back();
        }

        for (int i = 0; i < nprocess; ++i) {
            if (lambdas(n, i + 1) < r_values[i]) {
                count += 1;
            }
        }
    }

    for (int i = 0; i < count; ++i) {
        votes[outlier_indexes[i]] += 1;
    }
}

vector<int> generate_outlier_votes(const vector<double> & var,
                                   const size_t width,
                                   const size_t height) noexcept {
    vector<int> votes(width * height);

    for (size_t index = 0; index < width * height; ++index) {
        esd_test(var, width, height, index, votes);
    }

    return votes;
}
