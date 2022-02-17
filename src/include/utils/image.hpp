#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstring>
#include <fstream>
#include <string>
#include <vector>

// From src/include
#include <utils/vec3.hpp>

namespace kernels {
    // Identity kernel
    static constexpr double const identity[9] = { 0, 0, 0, 0, 1, 0, 0, 0, 0 };
    // Uniform blur kernel
    static constexpr double const box_blur[9] = {
        1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
        1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0
    };
    // Gaussian blur kernel
    static constexpr double const gaussian_blur[9] = { 0.0625, 0.125, 0.0625,
                                                       0.125,  0.25,  0.125,
                                                       0.0625, 0.125, 0.0625 };
    // Sharpening kernel
    static constexpr double const sharpen[9] = { 0,    -0.5, 0,    -0.5, 3,
                                                 -0.5, 0,    -0.5, 0 };
    // Embossing kernel
    static constexpr double const emboss[9] = { -2, -1, 0, -1, 1, 1, 0, 1, 2 };
} // namespace kernels

// Class for writing and reading PPM images. See the PPM specification at
// http://netpbm.sourceforge.net/doc/ppm.html
class Image {
public:
    // The pixels container type
    using container = std::vector<Colour>;

private:
    // A vector of pixels
    container data;

public:
    // Size of the image
    const int64_t width, height;

    // Create an empty image of size width * height
    inline Image(int64_t width, int64_t height) noexcept
        : data(container(width * height)), width(width), height(height) {}
    // Create an image of size width * height, using the pixels in the given
    // vector
    inline Image(container & data, int64_t width, int64_t height)
        : data(data), width(width), height(height) {
        if (data.size() > width * height) {
            throw "Could not construct Image: too many pixels in the given "
                  "vector.";
        } else {
            data.resize(width * height);
        }
    }
    // Create a black image of size width * height
    inline static Image black(int64_t width, int64_t height) noexcept {
        return Image(width, height);
    }

    // Parse a PPM image from a stream
    template <class charT, class charTraits = std::char_traits<charT>>
    static Image decode_ppm_stream(std::basic_istream<charT, charTraits> & is) {
        int64_t width, height;
        char buffer[8];
        uint16_t maxval;

        // Parsing PPM header, under the form
        // "P6 <width> <height> <max sample value>"(in ascii)
        if (is.getline(buffer, 3).good() && !std::strcmp(buffer, "P6")) {
            is >> width >> height >> maxval;
        } else {
            throw "Could not load PPM image: invalid magic number";
        }

        // Check stream health
        if (!is.good()) {
            throw "Could not load PPM image: invalid header";
        }

        // A single whitespace character between the header and the samples
        if (!std::isspace(is.get())) {
            throw "Could not load PPM image: unexpected character after "
                  "MAXVAL";
        }

        container pixels;
        int r, g, b;

        // Reserve appropriate space
        pixels.reserve(width * height);

        // Read samples from stream according to sample size;
        // one byte if maxval is one byte long, two if maxval is two bytes long.
        if (maxval < 256) {
            while (pixels.size() < width * height) {
                r = is.get();
                g = is.get();
                b = is.get();
                if (is.good()) {
                    pixels.push_back(Colour(
                        utils::reverse_gamma_correction(double(r) / maxval),
                        utils::reverse_gamma_correction(double(g) / maxval),
                        utils::reverse_gamma_correction(double(b) / maxval)));
                } else {
                    break;
                }
            }
        } else {
            while (pixels.size() < width * height) {
                r = (is.get() << 8) + is.get();
                g = (is.get() << 8) + is.get();
                b = (is.get() << 8) + is.get();
                if (is.good()) {
                    pixels.push_back(Colour(
                        utils::reverse_gamma_correction(double(r) / maxval),
                        utils::reverse_gamma_correction(double(g) / maxval),
                        utils::reverse_gamma_correction(double(b) / maxval)));
                } else {
                    break;
                }
            }
        }

        // Check that the right amount of pixels was read
        if (pixels.size() != width * height) {
            throw "Could not load PPM image: data size mismatch: found "
                + std::to_string(pixels.size())
                + " pixels where there should be "
                + std::to_string(width * height) + ".";
        }

        return Image(pixels, width, height);
    }

    // Load a PPM image from a filename
    inline static Image load_ppm(const std::string & filename) {
        std::ifstream file(filename, std::ios_base::in | std::ios_base::binary);
        if (!file) {
            throw "Could not load PPM image: could not open file!";
        }
        Image result = Image::decode_ppm_stream(file);
        file.close();
        return result;
    }

    // Get the image width
    constexpr int64_t get_width() const noexcept { return width; }
    // Get the image height
    constexpr int64_t get_height() const noexcept { return height; }
    // Get the image size
    inline int64_t size() const noexcept { return data.size(); }
    // Get the image container capacity
    inline int64_t capacity() const noexcept { return data.capacity(); }

    // Const vector access
    inline const Colour & operator[](size_t index) const noexcept {
        return data[index];
    }
    // Index the image by indexing the underlying vector
    inline Colour & operator[](size_t index) noexcept { return data[index]; }
    // Index the image by indexing the image matrix
    inline const Colour & at(size_t i, size_t j) const noexcept {
        return data[i * width + j];
    }
    inline Colour & at(size_t i, size_t j) noexcept {
        return data[i * width + j];
    }

    // Append a pixel to the image
    inline void push(Colour pixel) noexcept { data.push_back(pixel); }

    // Append a pixel by its normalized RGB components
    inline void push(double r, double g, double b) noexcept {
        data.push_back(Colour(r, g, b));
    }

    // Append each element of an iterator to the image
    template <class It>
    inline void push(const It & iterator) noexcept {
        for (Colour const & c : iterator) {
            data.push_back(c);
        }
    }

    // Write the image to a stream
    template <class charT, class charTraits = std::char_traits<charT>>
    void write_ppm(std::basic_ostream<charT, charTraits> & os) const {
        if (data.size() == width * height) {
            os << "P6" << std::endl
               << width << ' ' << height << std::endl
               << static_cast<uint16_t>(utils::MAX_COLOUR) << std::endl;

            for (const Colour & c : data) {
                c.write(os);
            }
        } else {
            throw "Could not write image to stream: Data size mismatch";
        }
    }

    // Save the image as PPM
    inline void save_ppm(const std::string & filename) const {
        std::ofstream output_file(filename,
                                  std::ios_base::out | std::ios_base::binary);
        if (!output_file) {
            throw "Could not open file!";
        }
        write_ppm(output_file);
        output_file.close();
    }

    // Save the image as PNG
    void save_png(const char * filename) const;

    // Compare two images for equality
    inline bool operator==(const Image & other) const noexcept {
        if (width != other.width || height != other.height
            || size() != other.size()) {
            return false;
        }

        int64_t data_size = size();
        for (int64_t i = 0; i < data_size; ++i) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }

        return true;
    }

    // Compare two images for inequality
    inline bool operator!=(const Image & other) const noexcept {
        return !(*this == other);
    }

    // Apply a 3x3 kernel filter to the given image.
    // The edge is handled by replicating pixels
    template <const double kernel[9]>
    void filter() {
        if (data.size() != width * height) {
            throw "Could not apply filter to incomplete image";
        }

        if (width < 2 || height < 2) {
            throw "Cannot apply filter: insufficient image size";
        }

        container old_line_data(width);
        container current_line_data(width);

        // Store the current line
        for (int64_t j = 0; j < width; ++j)
            current_line_data[j] = data[j];

        // Top left corner
        data[0] = (kernel[0] + kernel[1] + kernel[3] + kernel[4])
                      * current_line_data[0]
                  + (kernel[2] + kernel[5]) * current_line_data[1]
                  + (kernel[6] + kernel[7]) * data[width]
                  + kernel[8] * data[width + 1];
        // Top right corner
        data[width - 1] = (kernel[0] + kernel[3]) * current_line_data[width - 2]
                          + (kernel[1] + kernel[2] + kernel[4] + kernel[5])
                                * current_line_data[width - 1]
                          + kernel[6] * data[2 * width - 2]
                          + (kernel[7] + kernel[8]) * data[2 * width - 1];

        // Top row
        for (int64_t j = 1; j < width - 1; ++j) {
            data[j] = (kernel[0] + kernel[3]) * current_line_data[j - 1]
                      + (kernel[1] + kernel[4]) * current_line_data[j]
                      + (kernel[2] + kernel[5]) * current_line_data[j + 1]
                      + kernel[6] * data[width + j - 1]
                      + kernel[7] * data[width + j]
                      + kernel[8] * data[width + j + 1];
        }

        // Put current_line_data in old_line_data
        old_line_data.swap(current_line_data);

        for (int64_t i = 1; i < height - 1; ++i) {
            for (int64_t j = 0; j < width; ++j)
                current_line_data[j] = data[i * width + j];

            // Left column
            data[i * width] = (kernel[0] + kernel[1]) * old_line_data[0]
                              + kernel[2] * old_line_data[1]
                              + (kernel[3] + kernel[4]) * current_line_data[0]
                              + kernel[5] * current_line_data[1]
                              + (kernel[6] + kernel[7]) * data[(i + 1) * width]
                              + kernel[8] * data[(i + 1) * width + 1];
            // Right column
            data[(i + 1) * width - 1] =
                kernel[0] * old_line_data[width - 2]
                + (kernel[1] + kernel[2]) * old_line_data[width - 1]
                + kernel[3] * current_line_data[width - 2]
                + (kernel[4] + kernel[5]) * current_line_data[width - 1]
                + kernel[6] * data[(i + 2) * width - 2]
                + (kernel[7] + kernel[8]) * data[(i + 2) * width - 1];

            for (int64_t j = 1; j < width - 1; ++j) {
                data[i * width + j] =
                    kernel[0] * old_line_data[j - 1]
                    + kernel[1] * old_line_data[j]
                    + kernel[2] * old_line_data[j + 1]
                    + kernel[3] * current_line_data[j - 1]
                    + kernel[4] * current_line_data[j]
                    + kernel[5] * current_line_data[j + 1]
                    + kernel[6] * data[(i + 1) * width + j - 1]
                    + kernel[7] * data[(i + 1) * width + j]
                    + kernel[8] * data[(i + 1) * width + j + 1];
            }

            old_line_data.swap(current_line_data);
        }

        for (int64_t j = 0; j < width; ++j)
            current_line_data[j] = data[(height - 1) * width + j];

        // Bottom left corner
        data[(height - 1) * width] =
            (kernel[0] + kernel[1]) * old_line_data[0]
            + kernel[2] * old_line_data[1]
            + (kernel[3] + kernel[4] + kernel[6] + kernel[7])
                  * current_line_data[0]
            + (kernel[5] + kernel[8]) * current_line_data[1];
        // Bottom right corner
        data[height * width - 1] =
            kernel[0] * old_line_data[width - 2]
            + (kernel[1] + kernel[2]) * old_line_data[width - 1]
            + (kernel[3] + kernel[6]) * current_line_data[width - 2]
            + (kernel[4] + kernel[5] + kernel[7] + kernel[8])
                  * current_line_data[width - 1];

        // Bottom row
        for (int64_t j = 1; j < width - 1; ++j) {
            data[(height - 1) * width + j] =
                kernel[0] * old_line_data[j - 1] + kernel[1] * old_line_data[j]
                + kernel[2] * old_line_data[j + 1]
                + (kernel[3] + kernel[6]) * current_line_data[j - 1]
                + (kernel[4] + kernel[7]) * current_line_data[j]
                + (kernel[5] + kernel[8]) * current_line_data[j + 1];
        }
    }

    // Apply the box blur kernel filter in place
    inline void box_blur() { filter<kernels::box_blur>(); }

    // Apply the gaussian blur kernel filter in place
    inline void gaussian_blur() { filter<kernels::gaussian_blur>(); }

    // Apply the sharpen kernel filter in place
    inline void sharpen() { filter<kernels::sharpen>(); }

    // Apply the emboss kernel filter in place
    inline void emboss() { filter<kernels::emboss>(); }

    // Clamp the image colours in place
    inline void clamp() noexcept {
        for (Colour & c : data) {
            c = c.clamp();
        }
    }

    // Filter fireflies in an image given two variance half buffers
    //
    // Source: "Firefly Detection with Half Buffers", article by Keith Jeffery,
    // at DreamWorks (2018)
    void fireflies_filter(const std::vector<double> & var1,
                          const std::vector<double> & var2) noexcept;

private:
    // Compute the size of the intersection between a 5x5 kernel and the image
    // at the given position
    constexpr int patch_size(const size_t i, const size_t j) const noexcept {
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

    // Reconstruct the image without outliers using a 5x5 gaussian kernel
    void reconstruct(const size_t index, std::vector<bool> & outliers) noexcept;

public:
    // Construct an image from a single-value buffer
    static Image from_grayscale(const std::vector<double> & values,
                                const size_t width,
                                const size_t height) noexcept;
};

#endif
