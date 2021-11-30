#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstring>
#include <fstream>
#include <string>
#include <vector>

// From src/include
#include <utils/vec3.hpp>

namespace kernels {
    /* Identity kernel */
    constexpr double identity[9] = {0, 0, 0, 0, 1, 0, 0, 0, 0};
    /* Uniform blur kernel */
    constexpr double box_blur[9] = {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
                                    1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
                                    1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0};
    /* Gaussian blur kernel */
    constexpr double gaussian_blur[9] = {0.0625, 0.125,  0.0625, 0.125, 0.25,
                                         0.125,  0.0625, 0.125,  0.0625};
    /* Sharpening kernel */
    constexpr double sharpen[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
    /* Embossing kernel */
    constexpr double emboss[9] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};
} // namespace kernels

/* PPM images */
namespace image {

    // Class for writing and reading PPM images. See the PPM specification at
    // http://netpbm.sourceforge.net/doc/ppm.html
    class Image {
    private:
        /* A vector of pixels */
        std::vector<Colour> data;
        /* Size of the image */
        uint64_t width, height;

    public:
        /* Create an empty image of size width * height */
        inline Image(uint64_t width, uint64_t height) noexcept
            : data(std::vector<Colour>()), width(width), height(height) {
            data.reserve(width * height);
        }
        /* Create an image of size width * height, using the pixels in the given
         * vector */
        inline Image(std::vector<Colour> data, uint64_t width, uint64_t height)
            : data(data), width(width), height(height) {
            if (data.size() > width * height) {
                throw "Could not construct Image: too many pixels in the given "
                      "vector.";
            }
        }
        /* Create a black image of size width * height */
        inline static Image black(uint64_t width, uint64_t height) noexcept {
            return Image(std::vector<Colour>(width * height), width, height);
        }

        /* Parse a PPM image from a stream */
        template <class charT, class charTraits = std::char_traits<charT>>
        static Image decode_stream(std::basic_istream<charT, charTraits> & is) {
            uint64_t width, height;
            char buffer[8];
            uint16_t maxval;

            /* Parsing PPM header, under the form
            "P6 <width> <height> <max sample value>" (in ascii) */
            if (is.getline(buffer, 3).good() && !std::strcmp(buffer, "P6")) {
                is >> width >> height >> maxval;
            } else {
                throw "Could not load PPM image: invalid magic number";
            }

            /* Check stream health */
            if (!is.good()) {
                throw "Could not load PPM image: invalid header";
            }

            /* A single whitespace character between the header and the samples
             */
            if (!std::isspace(is.get())) {
                throw "Could not load PPM image: unexpected character after "
                      "MAXVAL";
            }

            std::vector<Colour> pixels;
            int r, g, b;

            /* Reserve appropriate space */
            pixels.reserve(width * height);

            /* Read samples from stream according to sample size;
            one byte if maxval is one byte long, two if maxval is two bytes
            long. */
            if (maxval < 256) {
                while (pixels.size() < width * height) {
                    r = is.get();
                    g = is.get();
                    b = is.get();
                    if (is.good()) {
                        pixels.push_back(Colour(
                            utils::reverse_gamma_correction(double(r) / maxval),
                            utils::reverse_gamma_correction(double(g) / maxval),
                            utils::reverse_gamma_correction(double(b)
                                                            / maxval)));
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
                            utils::reverse_gamma_correction(double(b)
                                                            / maxval)));
                    } else {
                        break;
                    }
                }
            }

            /* Check that the right amount of pixels was read */
            if (pixels.size() != width * height) {
                throw "Could not load PPM image: data size mismatch: found "
                    + std::to_string(pixels.size())
                    + " pixels where there should be "
                    + std::to_string(width * height) + ".";
            }

            return Image(pixels, width, height);
        }

        /* Load an image from a filename */
        inline static Image load(const std::string & filename) {
            std::ifstream file(filename,
                               std::ios_base::in | std::ios_base::binary);
            if (!file) {
                throw "Could not load PPM image: could not open file!";
            }
            Image result = Image::decode_stream(file);
            file.close();
            return result;
        }

        /* Get the image width */
        constexpr uint64_t get_width() const noexcept { return width; }
        /* Get the image height */
        constexpr uint64_t get_height() const noexcept { return height; }
        /* Get the image size */
        inline uint64_t size() const noexcept { return data.size(); }
        /* Get the image container capacity */
        inline uint64_t capacity() const noexcept { return data.capacity(); }

        /* Const vector access */
        inline const Colour & operator[](uint64_t index) const noexcept {
            return data[index];
        }
        /* Index the image by indexing the underlying vector */
        inline Colour & operator[](uint64_t index) noexcept {
            return data[index];
        }

        /* Append a pixel to the image */
        inline void push(Colour pixel) noexcept { data.push_back(pixel); }

        /* Append a pixel by its normalized RGB components */
        inline void push(double r, double g, double b) noexcept {
            data.push_back(Colour(r, g, b));
        }

        /* Append each element of an iterator to the image */
        template <class It>
        inline void push(const It & iterator) noexcept {
            for (Colour const & c : iterator) {
                data.push_back(c);
            }
        }

        /* Write the image to a stream */
        template <class charT, class charTraits = std::char_traits<charT>>
        void write(std::basic_ostream<charT, charTraits> & os) const {
            if (data.size() == width * height) {
                os << "P6" << std::endl
                   << width << ' ' << height << std::endl
                   << static_cast<uint16_t>(utils::MAX_COLOUR) << std::endl;

                for (Colour const & c : data) {
                    c.write(os);
                }
            } else {
                throw "Could not write image to stream: Data size mismatch";
            }
        }

        /* Save the image under the given filename */
        inline void save(const std::string & filename) const {
            std::ofstream output_file(filename, std::ios_base::out
                                                    | std::ios_base::binary);
            if (!output_file) {
                throw "Could not open file!";
            }
            write(output_file);
            output_file.close();
        }

        /* Compare two images for equality */
        inline bool operator==(const Image & other) const noexcept {
            if (width != other.width || height != other.height
                || size() != other.size()) {
                return false;
            }

            uint64_t data_size = size();
            for (uint64_t i = 0; i < data_size; ++i) {
                if (data[i] != other.data[i]) {
                    return false;
                }
            }

            return true;
        }

        /* Compare two images for inequality */
        inline bool operator!=(const Image & other) const noexcept {
            return !(*this == other);
        }

        /* Apply a 3x3 kernel filter to the given image */
        template <const double kernel[9]>
        Image filter() const {
            if (data.size() != width * height) {
                throw "Could not apply filter to incomplete image";
            }

            if (width < 2 || height < 2) {
                throw "Cannot apply filter: insufficient image size";
            }

            Image new_image = black(width, height);

            /* Top left corner */
            new_image[0] =
                (kernel[0] + kernel[1] + kernel[3] + kernel[4]) * data[0]
                + (kernel[2] + kernel[5]) * data[1]
                + (kernel[6] + kernel[7]) * data[width]
                + kernel[8] * data[width + 1];
            /* Top right corner */
            new_image[width - 1] =
                (kernel[0] + kernel[3]) * data[width - 2]
                + (kernel[1] + kernel[2] + kernel[4] + kernel[5])
                      * data[width - 1]
                + kernel[6] * data[2 * width - 2]
                + (kernel[7] + kernel[8]) * data[2 * width - 1];
            /* Bottom left corner */
            new_image[(height - 1) * width] =
                (kernel[0] + kernel[1]) * data[(height - 2) * width]
                + kernel[2] * data[(height - 2) * width + 1]
                + (kernel[3] + kernel[4] + kernel[6] + kernel[7])
                      * data[(height - 1) * width]
                + (kernel[5] + kernel[8]) * data[(height - 1) * width + 1];
            /* Bottom right corner */
            new_image[height * width - 1] =
                kernel[0] * data[(height - 1) * width - 2]
                + (kernel[1] + kernel[2]) * data[(height - 1) * width - 1]
                + (kernel[3] + kernel[6]) * data[height * width - 2]
                + (kernel[4] + kernel[5] + kernel[7] + kernel[8])
                      * data[height * width - 1];

            for (size_t i = 1; i < width - 1; ++i) {
                /* Top row */
                new_image[i] = (kernel[0] + kernel[3]) * data[i - 1]
                               + (kernel[1] + kernel[4]) * data[i]
                               + (kernel[2] + kernel[5]) * data[i + 1]
                               + kernel[6] * data[width + i - 1]
                               + kernel[7] * data[width + i]
                               + kernel[8] * data[width + i + 1];
                /* Bottom row */
                new_image[(height - 1) * width + i] =
                    kernel[0] * data[(height - 2) * width + i - 1]
                    + kernel[1] * data[(height - 2) * width + i]
                    + kernel[2] * data[(height - 2) * width + i + 1]
                    + (kernel[3] + kernel[6])
                          * data[(height - 1) * width + i - 1]
                    + (kernel[4] + kernel[7]) * data[(height - 1) * width + i]
                    + (kernel[5] + kernel[8])
                          * data[(height - 1) * width + i + 1];
            }

            for (size_t j = 1; j < height - 1; ++j) {
                /* Left column */
                new_image[j * width] =
                    (kernel[0] + kernel[1]) * data[(j - 1) * width]
                    + kernel[2] * data[(j - 1) * width + 1]
                    + (kernel[3] + kernel[4]) * data[j * width]
                    + kernel[5] * data[j * width + 1]
                    + (kernel[6] + kernel[7]) * data[(j + 1) * width]
                    + kernel[8] * data[(j + 1) * width + 1];
                /* Right column */
                new_image[(j + 1) * width - 1] =
                    kernel[0] * data[j * width - 2]
                    + (kernel[1] + kernel[2]) * data[j * width - 1]
                    + kernel[3] * data[(j + 1) * width - 2]
                    + (kernel[4] + kernel[5]) * data[(j + 1) * width - 1]
                    + kernel[6] * data[(j + 2) * width - 2]
                    + (kernel[7] + kernel[8]) * data[(j + 2) * width - 1];
            }

            /* Rest of the pixels */
            for (size_t i = 1; i < width - 1; ++i) {
                for (size_t j = 1; j < height - 1; ++j) {
                    new_image[j * width + i] =
                        kernel[0] * data[(j - 1) * width + i - 1]
                        + kernel[1] * data[(j - 1) * width + i]
                        + kernel[2] * data[(j - 1) * width + i + 1]
                        + kernel[3] * data[j * width + i - 1]
                        + kernel[4] * data[j * width + i]
                        + kernel[5] * data[j * width + i + 1]
                        + kernel[6] * data[(j + 1) * width + i - 1]
                        + kernel[7] * data[(j + 1) * width + i]
                        + kernel[8] * data[(j + 1) * width + i + 1];
                }
            }

            return new_image;
        }

        /* Apply the box blur kernel filter in place */
        inline void box_blur() { *this = filter<kernels::box_blur>(); }

        /* Apply the gaussian blur kernel filter in place */
        inline void gaussian_blur() {
            *this = filter<kernels::gaussian_blur>();
        }

        /* Apply the sharpen kernel filter in place */
        inline void sharpen() { *this = filter<kernels::sharpen>(); }

        /* Apply the emboss kernel filter in place */
        inline void emboss() { *this = filter<kernels::emboss>(); }
    };
} // namespace image

#endif
