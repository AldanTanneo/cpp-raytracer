#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstring>
#include <fstream>
#include <string>
#include <vector>

// From src/include
#include <utils/vec3.hpp>

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
    };
} // namespace image

#endif
