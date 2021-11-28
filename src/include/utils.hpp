#ifndef UTILS_HPP
#define UTILS_HPP

#include <atomic>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>
#include <type_traits>

namespace term_colours {
    constexpr char ESC = 27;
    constexpr char CYAN[6] = {ESC, '[', '3', '6', 'm', '\0'};
    constexpr char GREEN[6] = {ESC, '[', '3', '2', 'm', '\0'};
    constexpr char NO_COLOUR[5] = {ESC, '[', '0', 'm', '\0'};
    constexpr char BOLD[5] = {ESC, '[', '1', 'm', '\0'};
} // namespace term_colours

#define _DBG_COUT_IDENT                                                        \
    "[" __FILE__ ":" << __FUNCTION__ << ":" << __LINE__ << "] "

#define STRINGIFY(args) #args

#ifdef _DISABLE_DEBUGGING
    #define DEBUG(...) ;
#else
    /* Debugging macro */
    #define DEBUG(args, ...)                                                   \
        __VA_OPT__(std::cout << term_colours::CYAN << term_colours::BOLD       \
                             << _DBG_COUT_IDENT << term_colours::NO_COLOUR     \
                             << term_colours::BOLD << __VA_ARGS__              \
                             << term_colours::NO_COLOUR << std::endl;)         \
        std::cout << term_colours::CYAN << term_colours::BOLD                  \
                  << _DBG_COUT_IDENT << term_colours::NO_COLOUR << (#args)     \
                  << " = " << (args) << std::endl;
#endif

namespace utils {
    /* Outputs a newline */
    inline void newline() { fputs("\n", stdout); }

    // Returns the number of utf8 code points in the buffer at s
    constexpr size_t utf8len(const char * s) {
        size_t len = 0;
        for (; *s; ++s)
            if ((*s & 0xC0) != 0x80)
                ++len;
        return len;
    }

    /* Maximum floating point color value. Used for conversion between floating
     * point and integer color. */
    constexpr double MAX_COLOUR = 255.999999;
    /* Common epsilon for floating point uses */
    constexpr double EPSILON = 1e-9;
    /* Infinity */
    constexpr double INF = std::numeric_limits<double>::infinity();
    /* Geometric PI constant */
    constexpr double PI = 3.14159265358979323846264338327950288;

    template <class T>
    constexpr std::enable_if_t<std::is_floating_point_v<T>, T>
    to_radians(T value) noexcept {
        return value * static_cast<T>(PI) / 180.0;
    }

    /* Clamps a floating point value to the [0.0, 1.0] range */
    template <class T>
    constexpr std::enable_if_t<std::is_floating_point_v<T>, T>
    clamp(const T value) noexcept {
        if (value < 0.0)
            return static_cast<T>(0.0);
        if (value > 1.0)
            return static_cast<T>(1.0);
        return value;
    }

    /* Clamps an ordered value to the [min, max] range. */
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

    /* Gamma corrects the colour for writing to a file */
    inline double gamma_correction(double sample) noexcept {
        return sqrt(clamp(sample));
    }

    /* Progress bar class */
    class ProgressBar {
    private:
        /* The current position of the progress bar */
        std::atomic<size_t> progress;
        /* The foreground character */
        const char * fchar;
        /* The background character */
        const char * bchar;
        /* Subdivisions of the progress bar */
        const size_t subdiv;
        /* Draw interval */
        const size_t delta;

    public:
        /* Construct a default progress bar with 100 subdivisions */
        inline ProgressBar(size_t size) noexcept
            : fchar("▓"), bchar("░"), subdiv(100), delta(size / 100) {
            progress.store(0);
        }
        /* Constructs a progress bar, specify the number of subdivisions. Draws
        two characters for each subdivision */
        inline ProgressBar(size_t size, size_t subdiv) noexcept
            : fchar("▓▓"), bchar("░░"), subdiv(subdiv), delta(size / subdiv) {
            progress.store(0);
        }
        /* Construct a custom progress bar. Specify the number of subdivisions
        and the drawn characters. The "bchar" and "fchar" strings must have the
        same UTF-8 length */
        inline ProgressBar(size_t size,
                           size_t subdiv,
                           const char * fchar,
                           const char * bchar)
            : fchar(fchar), bchar(bchar), subdiv(subdiv), delta(size / subdiv) {
            if (utf8len(fchar) != utf8len(bchar)) {
                throw "Foreground and background characters must have the same "
                      "UTF-8 length";
            }
            progress.store(0);
        }
        /* Delete the copy constructor */
        ProgressBar(const ProgressBar &) = delete;

        /* Set the current position */
        inline void set_state(size_t state) noexcept { progress.store(state); }

        /* Writes the empty progress bar to stdout and returns to line start */
        inline void start() const noexcept {
            size_t chars_to_write = subdiv * utf8len(bchar);
            for (size_t i = 0; i < subdiv; i++)
                fputs(bchar, stdout);
            for (size_t i = 0; i <= chars_to_write; i++)
                fputc('\b', stdout);
        }

        /* Writes the empty progress bar to stdout, with the given style. */
        inline void start(const char * style) const noexcept {
            fputs(term_colours::NO_COLOUR, stdout);
            fputs(style, stdout);
            start();
        }

        inline void advance() noexcept {
            size_t p = ++progress;
            if (p % delta == 0)
                fputs(fchar, stdout);
        }

        inline void operator++(int) noexcept { advance(); }

        inline void stop() const noexcept { puts(term_colours::NO_COLOUR); }
    };

} // namespace utils

#endif
