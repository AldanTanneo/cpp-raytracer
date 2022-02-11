#ifndef FIREFLIES_FILTER_HPP
#define FIREFLIES_FILTER_HPP

#include <vector>

// From src/include
#include <utils/image.hpp>

Image fireflies_filter(Image & img1,
                       const std::vector<double> & var1,
                       Image & img2,
                       const std::vector<double> & var2) noexcept;

#endif
