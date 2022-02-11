#include <algorithm>
#include <cmath>
#include <cstdint>
#include <utility>

// From src/include
#include <utils/image.hpp>

// External PNG library
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <extern/stb_image_write.hpp>

void Image::save_png(const char * filename) const {
    if (data.size() != width * height) {
        throw "Could not write image to PNG: Data size mismatch";
    }
    std::vector<uint8_t> raw_data(width * height * 3);

    for (size_t i = 0; i < width * height; ++i) {
        Colour c = data[i];
        raw_data[3 * i] = c.red();
        raw_data[3 * i + 1] = c.green();
        raw_data[3 * i + 2] = c.blue();
    }

    if (!stbi_write_png(filename, width, height, 3, raw_data.data(),
                        width * 3)) {
        throw "Could not save png image!";
    }
}

Image Image::from_grayscale(const std::vector<double> & values,
                            const size_t width,
                            const size_t height) noexcept {
    const auto test = std::minmax_element(values.cbegin(), values.cend());
    const double min = *test.first, max = *test.second;

    Image res(width, height);

    for (size_t index = 0; index < width * height; ++index) {
        res[index] =
            colour::WHITE * std::sqrt((values[index] - min) / (max - min));
    }

    return res;
}
