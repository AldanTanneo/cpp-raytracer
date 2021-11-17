#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <algorithm>
#include <fstream>
#include <ostream>
#include <vec3.hpp>
#include <vector>

using colour::Colour;

// Class for writing PPM images
class Image {
   private:
    std::vector<Colour> data;
    uint64_t width, height;

   public:
    Image() : data(std::vector<Colour>()), width(0), height(0) {}
    Image(uint64_t width, uint64_t height) : data(std::vector<Colour>()), width(width), height(height) {
        data.reserve(width * height);
    }
    Image(std::vector<Colour> data, uint64_t width, uint64_t height) : data(data), width(width), height(height) {}

    inline uint64_t get_width() const { return width; }
    inline uint64_t get_height() const { return height; }

    inline Colour operator[](uint64_t index) const {
        return data[index];
    }

    inline void push(Colour pixel) {
        data.push_back(pixel);
    }

    inline void push(float r, float g, float b) {
        data.push_back(Colour(r, g, b));
    }

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

    inline void save(const std::string & filename) const {
        std::fstream output_file(filename, std::ios_base::out | std::ios_base::binary);
        write(output_file);
        output_file.close();
    }
};

#endif
