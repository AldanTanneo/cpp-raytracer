#ifndef UTILS_HPP
#define UTILS_HPP

#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>
#include <type_traits>

namespace term_colours {
    constexpr char const * RESET = "\x1B[0m";

    /* Font styles */
    constexpr char const * BOLD = "\x1B[1m";
    constexpr char const * DIM = "\x1B[2m";
    constexpr char const * ITALIC = "\x1B[3m";
    constexpr char const * UNDERLINE = "\x1B[4m";
    constexpr char const * BLINKING = "\x1B[5m";
    constexpr char const * INVERSE = "\x1B[7m";
    constexpr char const * HIDDEN = "\x1B[8m";
    constexpr char const * STRIKETHROUGH = "\x1B[9m";
    constexpr char const * DOUBLE_UNDERLINE = "\x1B[21m";

    /* Font styles reset */
    constexpr char const * NO_BOLD = "\x1B[22m";
    constexpr char const * NO_DIM = "\x1B[22m";
    constexpr char const * NO_ITALIC = "\x1B[23m";
    constexpr char const * NO_UNDERLINE = "\x1B[24m";
    constexpr char const * NO_BLINKING = "\x1B[25m";
    constexpr char const * NO_INVERSE = "\x1B[27m";
    constexpr char const * NO_HIDDEN = "\x1B[28m";
    constexpr char const * NO_STRIKETHROUGH = "\x1B[29m";

    /* Foreground colours */
    constexpr char const * BLACK = "\x1B[30m";
    constexpr char const * RED = "\x1B[31m";
    constexpr char const * GREEN = "\x1B[32m";
    constexpr char const * YELLOW = "\x1B[33m";
    constexpr char const * BLUE = "\x1B[34m";
    constexpr char const * MAGENTA = "\x1B[35m";
    constexpr char const * CYAN = "\x1B[36m";
    constexpr char const * WHITE = "\x1B[37";
    constexpr char const * DEFAULT_FOREGROUND = "\x1B[39m";

    /* Background colours */
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

#define _DBG_COUT_IDENT                                                        \
    "[" __FILE__ ":" << __FUNCTION__ << ":" << __LINE__ << "] "

#define STRINGIFY(args) #args

#ifdef _DISABLE_DEBUGGING
    #define DEBUG(...) ;
#else
    /* Debugging macro */
    #define DEBUG(args, ...)                                                   \
        __VA_OPT__(std::cout                                                   \
                       << term_colours::CYAN << term_colours::BOLD             \
                       << _DBG_COUT_IDENT << term_colours::DEFAULT_FOREGROUND  \
                       << __VA_ARGS__ << term_colours::NO_BOLD << std::endl;)  \
        std::cout << term_colours::CYAN << term_colours::BOLD                  \
                  << _DBG_COUT_IDENT << term_colours::DEFAULT_FOREGROUND       \
                  << term_colours::NO_BOLD << (#args) << " = " << (args)       \
                  << std::endl;
#endif

namespace utils {
    /* Outputs a newline */
    inline void newline() {
        fputc('\n', stdout);
        fflush(stdout);
    }

    /* Returns the number of utf8 code points in the buffer at s */
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

    /* Gamma corrects the colour sample for writing to a file */
    inline double gamma_correction(double sample) noexcept {
        return sqrt(clamp(sample));
    }

    /* Reverse gamma correction for reading an external colour */
    constexpr double reverse_gamma_correction(double sample) noexcept {
        return sample * sample;
    }

    /* Compute reflectance according to Schlick's approximation */
    inline double reflectance(double cos_theta,
                              double refraction_ratio) noexcept {
        double r0 = (1.0 - refraction_ratio) / (1.0 + refraction_ratio);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * pow(1.0 - cos_theta, 5);
    }

    /* Progress bar class */
    class ProgressBar {
    private:
        /* Clock used to measure total elapsed time */
        using clock = std::chrono::system_clock;
        /* Time point type */
        using time_point = std::chrono::time_point<clock>;
        /* Default bar subdivision */
        static constexpr size_t default_subdiv = 80;

        /* The total size of the progress bar */
        const size_t size;
        /* The current position of the progress bar */
        std::atomic<size_t> progress;
        /* The foreground character */
        const char * fchar;
        /* The background character */
        const char * bchar;
        /* Subdivisions of the progress bar */
        const size_t subdiv;
        /* Start time */
        const time_point start_time;

    public:
        /* Construct a default progress bar with 80 subdivisions */
        inline ProgressBar(size_t size) noexcept
            : size(size), fchar("▓"), bchar("░"), subdiv(default_subdiv),
              start_time(clock::now()) {
            progress.store(0);
        }
        /* Constructs a progress bar, specify the number of subdivisions. */
        inline ProgressBar(size_t size, size_t subdiv) noexcept
            : size(size), fchar("▓"), bchar("░"), subdiv(subdiv),
              start_time(clock::now()) {
            progress.store(0);
        }
        /* Construct a custom progress bar. Specify the number of subdivisions
        and the drawn characters. The "bchar" and "fchar" strings must have the
        same UTF-8 length */
        inline ProgressBar(size_t size,
                           size_t subdiv,
                           const char * fchar,
                           const char * bchar)
            : size(size), fchar(fchar), bchar(bchar), subdiv(subdiv),
              start_time(clock::now()) {
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

        /* Writes the empty progress bar to stdout and returns to line start.
        Does not reset the terminal style. */
        inline void start() const noexcept {
            for (size_t i = 0; i < subdiv; ++i)
                fputs(bchar, stdout);
            size_t bchar_width = utf8len(bchar);
            for (size_t i = 0; i < subdiv * bchar_width; ++i)
                fputc('\b', stdout);
            fflush(stdout);
        }

        /* Writes the empty progress bar to stdout, with the given style. */
        inline void start(const char * style) const noexcept {
            fputs(term_colours::RESET, stdout);
            fputs(style, stdout);
            start();
        }

        /* Advance the progress bar by one, redraw if needed */
        inline void advance() noexcept {
            if ((subdiv * progress.fetch_add(1)) % size == 0) {
                fputs(fchar, stdout);
                fflush(stdout);
            }
        }

        /* Advance the progress bar by n, redraw if needed */
        inline void advance(size_t n) noexcept {
            const size_t old_value = progress.fetch_add(n);
            const size_t redraws =
                subdiv * (n + (subdiv * old_value % size)) / size;
            for (size_t i = 0; i < redraws; ++i)
                fputs(fchar, stdout);
            fflush(stdout);
        }

        /* Set back the progress bar by one, redraw if needed */
        inline void setback() noexcept {
            const size_t fchar_width = utf8len(fchar);
            const size_t bchar_width = utf8len(bchar);
            if ((subdiv * progress.fetch_sub(1)) % size == 0) {
                for (size_t i = 0; i < fchar_width; ++i)
                    fputc('\b', stdout);
                fputs(bchar, stdout);
                for (size_t i = 0; i < bchar_width; ++i)
                    fputc('\b', stdout);
                fflush(stdout);
            }
        }

        /* Set back the progress bar by n, redraw if needed */
        inline void setback(size_t n) noexcept {
            const size_t fchar_width = utf8len(fchar);
            const size_t bchar_width = utf8len(bchar);
            const size_t old_value = progress.fetch_sub(n);
            const size_t redraws =
                (subdiv * n + size - old_value % size) / size;
            for (size_t i = 0; i < redraws; ++i) {
                for (size_t j = 0; j < fchar_width; ++j)
                    fputc('\b', stdout);
                fputs(bchar, stdout);
                for (size_t j = 0; j < bchar_width; ++j)
                    fputc('\b', stdout);
            }
            fflush(stdout);
        }

        /* Stop the progress bar, print a message with the total elapsed time */
        inline void stop(const char * message = "Done") const noexcept {
            using namespace std::chrono;
            int64_t millis =
                duration_cast<milliseconds>(clock::now() - start_time).count();
            std::cout << term_colours::RESET << std::endl
                      << message << " in " << millis / 1000 << "s "
                      << millis % 1000 << "ms" << std::endl;
            fflush(stdout);
        }

        /* Stop the progress bar, do not print a message */
        inline void stop_and_leave() const noexcept {
            puts(term_colours::RESET);
            fflush(stdout);
        }
    };

} // namespace utils

#endif
