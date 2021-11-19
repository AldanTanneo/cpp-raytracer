#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

// From src/include
#include <vec3.hpp>

using colour::Colour;

// Class for writing PPM images
class Image {
   private:
    /* A vector of pixels */
    std::vector<Colour> data;
    /* Size of the image */
    uint64_t width, height;

   public:
    /* Create an empty image of size width * height */
    inline Image(uint64_t width, uint64_t height) : data(std::vector<Colour>()), width(width), height(height) {
        data.reserve(width * height);
    }
    /* Create an image of size width * height, using the pixels in the given vector */
    inline Image(std::vector<Colour> data, uint64_t width, uint64_t height) : data(data), width(width), height(height) {}

    template <class charT, class charTraits = std::char_traits<charT>>
    static Image decode_stream(std::basic_istream<charT, charTraits> & is) {
        uint64_t width, height, maxval;
        char buffer[8];

        if (is.getline(buffer, 3).good() && !std::strcmp(buffer, "P6")) {
            is >> width >> height >> maxval;
        } else {
            throw "Could not load PPM image: header not found.";
        }

        if (!std::isspace(is.get())) {
            throw "Could not load PPM image: unexpected character after MAXVAL";
        }

        std::vector<Colour> pixels;
        int r, g, b;

        pixels.reserve(width * height);

        if (maxval < 256) {
            while (1) {
                r = is.get();
                g = is.get();
                b = is.get();
                if (is.good()) {
                    pixels.push_back(Colour(float(r) / maxval, float(g) / maxval, float(b) / maxval));
                } else {
                    break;
                }
            }
        } else {
            while (1) {
                r = (is.get() << 8) + is.get();
                g = (is.get() << 8) + is.get();
                b = (is.get() << 8) + is.get();
                if (is.good()) {
                    pixels.push_back(Colour(float(r) / maxval, float(g) / maxval, float(b) / maxval));
                } else {
                    break;
                }
            }
        }

        if (pixels.size() != width * height) {
            throw "Could not load PPM image: data size mismatch: found " + std::to_string(pixels.size()) + " pixels where there should be " + std::to_string(width * height) + ".";
        }

        return Image(pixels, width, height);
    }

    inline static Image load(const std::string & filename) {
        std::ifstream file(filename, std::ios_base::in | std::ios_base::binary);
        if (!file) {
            throw "Could not open file!";
        }
        Image result = Image::decode_stream(file);
        file.close();
        return result;
    }

    /* Get the image width */
    inline uint64_t get_width() const { return width; }
    /* Get the image height */
    inline uint64_t get_height() const { return height; }
    /* Get the image size */
    inline uint64_t size() const { return data.size(); }
    /* Get the image container capacity */
    inline uint64_t capacity() const { return data.capacity(); }

    /* Index the image by indexing the underlying vector */
    inline Colour operator[](uint64_t index) const {
        return data[index];
    }

    /* Append a pixel to the image */
    inline void push(Colour pixel) {
        data.push_back(pixel);
    }

    /* Append a pixel by its (r, g, b) components */
    inline void push(float r, float g, float b) {
        data.push_back(Colour(r, g, b));
    }

    /* Append each element of an iterator to the image */
    template <class It>
    inline void push(It & iterator) {
        std::for_each(std::begin(iterator), std::end(iterator), [this](Colour const & c) {
            data.push_back(c);
        });
    }

    /* Write the image to a stream */
    template <class charT, class charTraits = std::char_traits<charT>>
    void write(std::basic_ostream<charT, charTraits> & os) const {
        if (data.size() == width * height) {
            os << "P6" << std::endl
               << width << ' ' << height << std::endl
               << "255" << std::endl;

            std::for_each(std::begin(data), std::end(data), [&os](Colour const & c) {
                c.write(os);
            });
        } else {
            throw "Could not write image to stream: Data size mismatch";
        }
    }

    /* Save the image under the given filename */
    inline void save(const std::string & filename) const {
        std::ofstream output_file(filename, std::ios_base::out | std::ios_base::binary);
        if (!output_file) {
            throw "Could not open file!";
        }
        write(output_file);
        output_file.close();
    }

    inline bool operator==(const Image & other) const {
        if (width != other.width || height != other.height || size() != other.size()) {
            return false;
        }

        Colour c1, c2;
        for (uint64_t i = 0; i < size(); i++) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }

        return true;
    }

    inline bool operator!=(const Image & other) const {
        return !(*this == other);
    }
};

#endif
