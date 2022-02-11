#include <omp.h>
#include <vector>

// From src/include
#include <fireflies/fireflies_filter.hpp>
#include <fireflies/generalized_esd_test.hpp>
#include <utils.hpp>
#include <utils/image.hpp>

static const constinit double gaussian_5x5_kernel[5][5] = {{1, 4, 7, 4, 1},
                                                           {4, 16, 26, 16, 4},
                                                           {7, 26, 41, 26, 7},
                                                           {4, 16, 26, 16, 4},
                                                           {1, 4, 7, 4, 1}};

constexpr int
patch_size(size_t i, size_t j, size_t width, size_t height) noexcept {
    int w = 5, h = 5;
    if (j < 2)
        w -= 2 - j;
    if (j + 3 > width)
        w -= j + 3 - width;
    if (i < 2)
        h -= 2 - i;
    if (i + 3 > height)
        h -= i + 3 - height;
    return w * h;
}

void reconstruct(Image & img,
                 const size_t index,
                 std::vector<bool> & outliers) {
    // Do not count the pixel self in its reconstruction computation
    outliers[index] = true;

    std::vector<IndexedValue> kernel;
    double kernel_sum = 0.0;

    const size_t y = index / img.width;
    const size_t x = index % img.width;

    const size_t left = std::max((size_t)2, x) - 2;
    const size_t right = std::min((size_t)img.width, x + 3);
    const size_t top = std::max((size_t)2, y) - 2;
    const size_t bottom = std::min((size_t)img.height, y + 3);

    for (size_t j = left; j < right; ++j) {
        for (size_t i = top; i < bottom; ++i) {
            size_t ind = i * img.width + j;
            if (!outliers[ind]) {
                double kernel_val = gaussian_5x5_kernel[x + 2 - j][y + 2 - i];

                kernel.push_back(IndexedValue(kernel_val, ind));
                kernel_sum += kernel_val;
            }
        }
    }

    img[index] = colour::BLACK;
    for (const IndexedValue & k : kernel) {
        img[index] += img[k.index] * k.value;
    }
    img[index] /= kernel_sum;

    // Once the pixel has been reconstructed, remove it from the outliers.
    outliers[index] = false;
}

Image fireflies_filter(Image & img1,
                       const std::vector<double> & var1,
                       Image & img2,
                       const std::vector<double> & var2) noexcept {
    const size_t width = img1.width;
    const size_t height = img1.height;

    const size_t n = width * height;

    std::cout << "Identifying outliers..." << std::endl;

    std::vector<int> votes1 = generate_outlier_votes(var1, width, height),
                     votes2 = generate_outlier_votes(var2, width, height);

    DEBUG(votes1[1000 * 750 + 250]);

    std::vector<bool> outliers(n, false);
    std::vector<size_t> outlier_indexes;

    for (size_t index = 0; index < n; ++index) {
        const size_t y = index / width;
        const size_t x = index % width;
        const int threshold = patch_size(y, x, width, height) - 2;

        if ((votes1[index] > threshold) ^ (votes2[index] > threshold)) {
            outliers[index] = true;
            outlier_indexes.push_back(index);
        }
    }

    std::cout << "Successfully identified " << outlier_indexes.size()
              << " outliers." << std::endl;

    Image img(width, height);
    for (size_t index = 0; index < width * height; ++index) {
        img[index] = img1[index] + img2[index];
    }

    std::cout << "Reconstructing outlier pixels... ";
    // First reconstruction pass: ignore outliers
    for (const size_t & index : outlier_indexes) {
        reconstruct(img, index, outliers);
    }

    // Second reconstruction pass: count outliers for convergence
    for (int i = 0; i < 10; ++i) {
        for (const size_t & index : outlier_indexes) {
            reconstruct(img, index, outliers);
        }
    }

    std::cout << "Done." << std::endl;

    return img;
}
