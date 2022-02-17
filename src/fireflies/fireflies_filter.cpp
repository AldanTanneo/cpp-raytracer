#include <omp.h>
#include <vector>

// From src/include
#include <fireflies/generalized_esd_test.hpp>
#include <utils.hpp>
#include <utils/image.hpp>

static const constinit double gaussian_5x5_kernel[5][5] = {
    { 1, 4, 7, 4, 1 },
    { 4, 16, 26, 16, 4 },
    { 7, 26, 41, 26, 7 },
    { 4, 16, 26, 16, 4 },
    { 1, 4, 7, 4, 1 }
};

void Image::reconstruct(const size_t index,
                        std::vector<bool> & outliers) noexcept {
    // Do not count the pixel self in its reconstruction computation
    outliers[index] = true;

    std::vector<IndexedValue> kernel;
    double kernel_sum = 0.0;

    const size_t y = index / width;
    const size_t x = index % width;

    const size_t left = std::max((size_t)2, x) - 2;
    const size_t right = std::min((size_t)width - 1, x + 2) + 1;
    const size_t top = std::max((size_t)2, y) - 2;
    const size_t bottom = std::min((size_t)height - 1, y + 2) + 1;

    for (size_t j = left; j < right; ++j) {
        for (size_t i = top; i < bottom; ++i) {
            size_t ind = i * width + j;
            if (!outliers[ind]) {
                double kernel_val = gaussian_5x5_kernel[x + 2 - j][y + 2 - i];

                kernel.push_back(IndexedValue(kernel_val, ind));
                kernel_sum += kernel_val;
            }
        }
    }

    data[index] = colour::BLACK;
    for (const IndexedValue & k : kernel) {
        data[index] += data[k.index] * k.value;
    }
    data[index] /= kernel_sum;

    // Once the pixel has been reconstructed, remove it from the outliers.
    outliers[index] = false;
}

void Image::fireflies_filter(const std::vector<double> & var1,
                             const std::vector<double> & var2) noexcept {
    const size_t n = width * height;

    std::cout << "Identifying outliers..." << std::endl;

    std::vector<int> votes1 = generate_outlier_votes(var1, width, height),
                     votes2 = generate_outlier_votes(var2, width, height);

    std::vector<bool> outliers(n, false);
    std::vector<size_t> outlier_indexes;

    for (size_t index = 0; index < n; ++index) {
        const size_t i = index / width;
        const size_t j = index % width;
        const int threshold = patch_size(i, j) - 1;

        if ((votes1[index] >= threshold) ^ (votes2[index] >= threshold)) {
            outliers[index] = true;
            outlier_indexes.push_back(index);
        }
    }

    std::cout << "Successfully identified " << outlier_indexes.size()
              << " outliers." << std::endl;

    std::cout << "Reconstructing outlier pixels... ";
    // First reconstruction pass: ignore outliers
    for (const size_t & index : outlier_indexes) {
        reconstruct(index, outliers);
    }

    // Second reconstruction pass: count outliers for convergence
    for (int i = 0; i < 10; ++i) {
        for (const size_t & index : outlier_indexes) {
            reconstruct(index, outliers);
        }
    }

    std::cout << "Done." << std::endl;
}
