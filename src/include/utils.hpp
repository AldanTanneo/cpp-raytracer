#ifndef UTILS_HPP
#define UTILS_HPP

#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>
#include <type_traits>

// From src/include
#include <extern/gcem.hpp>

namespace term_colours {
    constexpr char const * RESET = "\x1B[0m";

    // Font styles
    constexpr char const * BOLD = "\x1B[1m";
    constexpr char const * DIM = "\x1B[2m";
    constexpr char const * ITALIC = "\x1B[3m";
    constexpr char const * UNDERLINE = "\x1B[4m";
    constexpr char const * BLINKING = "\x1B[5m";
    constexpr char const * INVERSE = "\x1B[7m";
    constexpr char const * HIDDEN = "\x1B[8m";
    constexpr char const * STRIKETHROUGH = "\x1B[9m";
    constexpr char const * DOUBLE_UNDERLINE = "\x1B[21m";

    // Font styles reset
    constexpr char const * NO_BOLD = "\x1B[22m";
    constexpr char const * NO_DIM = "\x1B[22m";
    constexpr char const * NO_ITALIC = "\x1B[23m";
    constexpr char const * NO_UNDERLINE = "\x1B[24m";
    constexpr char const * NO_BLINKING = "\x1B[25m";
    constexpr char const * NO_INVERSE = "\x1B[27m";
    constexpr char const * NO_HIDDEN = "\x1B[28m";
    constexpr char const * NO_STRIKETHROUGH = "\x1B[29m";

    // Foreground colours
    constexpr char const * BLACK = "\x1B[30m";
    constexpr char const * RED = "\x1B[31m";
    constexpr char const * GREEN = "\x1B[32m";
    constexpr char const * YELLOW = "\x1B[33m";
    constexpr char const * BLUE = "\x1B[34m";
    constexpr char const * MAGENTA = "\x1B[35m";
    constexpr char const * CYAN = "\x1B[36m";
    constexpr char const * WHITE = "\x1B[37";
    constexpr char const * DEFAULT_FOREGROUND = "\x1B[39m";

    // Background colours
    constexpr char const * BLACK_BACKGROUND = "\x1B[40m";
    constexpr char const * RED_BACKGROUND = "\x1B[41m";
    constexpr char const * GREEN_BACKGROUND = "\x1B[42m";
    constexpr char const * YELLOW_BACKGROUND = "\x1B[43m";
    constexpr char const * BLUE_BACKGROUND = "\x1B[44m";
    constexpr char const * MAGENTA_BACKGROUND = "\x1B[45m";
    constexpr char const * CYAN_BACKGROUND = "\x1B[46m";
    constexpr char const * WHITE_BACKGROUND = "\x1B[47";
    constexpr char const * DEFAULT_BACKGROUND = "\x1B[49m";
} // namespace term_colours

#define STRINGIFY(args) #args

#ifdef DISABLE_DEBUGGING
    #define DEBUG(...) ;
#else
  // Debugging macro
    #define DEBUG(args, ...)                                                   \
        __VA_OPT__(std::cout                                                   \
                       << term_colours::CYAN << term_colours::BOLD             \
                       << "[" __FILE__ ":" << __FUNCTION__ << ":" << __LINE__  \
                       << "] " << term_colours::DEFAULT_FOREGROUND             \
                       << __VA_ARGS__ << term_colours::NO_BOLD << std::endl;)  \
        std::cout << term_colours::CYAN << term_colours::BOLD                  \
                  << "[" __FILE__ ":" << __FUNCTION__ << ":" << __LINE__       \
                  << "] " << term_colours::DEFAULT_FOREGROUND                  \
                  << term_colours::NO_BOLD << (#args) << " = " << (args)       \
                  << std::endl;
#endif

namespace log {
    // Log output to stdout
    inline void message(const std::string & msg) noexcept {
        std::cout << msg << std::endl;
    }
    // Log warnings to stderr
    inline void warn(const std::string & msg) noexcept {
        std::cerr << term_colours::YELLOW << term_colours::BOLD
                  << "[WARNING]: " << term_colours::NO_BOLD
                  << term_colours::DEFAULT_FOREGROUND << msg << std::endl;
    }

    // Log errors to stderr
    inline void error(const std::string & msg) noexcept {
        std::cerr << term_colours::RED << term_colours::BOLD
                  << "[ERROR]: " << term_colours::NO_BOLD
                  << term_colours::DEFAULT_FOREGROUND << msg << std::endl;
    }
} // namespace log

namespace utils {
    // Outputs a newline
    inline void newline() {
        fputc('\n', stdout);
        fflush(stdout);
    }

    // Returns the number of utf8 code points in the buffer at s
    constexpr size_t utf8len(const char * s) {
        size_t len = 0;
        for (; *s; ++s)
            if ((*s & 0xC0) != 0x80)
                ++len;
        return len;
    }

    // Maximum floating point color value. Used for conversion between floating
    // point and integer color.
    constexpr double MAX_COLOUR = 255.999999;
    // Common epsilon for floating point uses
    constexpr double EPSILON = 1e-8;
    // Infinity
    constexpr double INF = std::numeric_limits<double>::infinity();
    // Geometric PI constant
    constexpr double PI = 3.14159265358979323846264338327950288;
    // Geometric TAU constant, TAU = 2.0 * PI
    constexpr double TAU = 6.28318530717958647692528676655900577;

    // Convert a value in degrees to radians
    template <class T>
    requires std::is_floating_point_v<T>
    constexpr T to_radians(T value) noexcept {
        return value * static_cast<T>(PI) / 180.0;
    }

    // Compute the absolute value of a number
    template <class T>
    constexpr T abs(const T value) {
        if (value < 0) {
            return -value;
        }
        return value;
    }

    // Compute a square root at compile time.
    consteval double const_sqrt(const double x) { return gcem::sqrt(x); }

    // Clamps a floating point value to the [0.0, 1.0] range
    template <class T>
    requires std::is_floating_point_v<T>
    constexpr T clamp(const T value) noexcept {
        if (value < 0.0)
            return static_cast<T>(0.0);
        if (value > 1.0)
            return static_cast<T>(1.0);
        return value;
    }

    // Clamps an ordered value to the [min, max] range.
    template <class T>
    constexpr T clamp(const T value, const T min, const T max) noexcept {
        if (value < min) {
            return min;
        }
        if (value > max) {
            return max;
        }
        return value;
    }

    // Gamma corrects the colour sample for writing to a file
    inline double gamma_correction(double sample) noexcept {
        return sqrt(clamp(sample));
    }

    // Reverse gamma correction for reading an external colour
    constexpr double reverse_gamma_correction(double sample) noexcept {
        return sample * sample;
    }

    // Compute reflectance according to Schlick's approximation
    inline double reflectance(double cos_theta,
                              double refraction_ratio) noexcept {
        double r0 = (1.0 - refraction_ratio) / (1.0 + refraction_ratio);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * pow(1.0 - cos_theta, 5);
    }

} // namespace utils

#endif
