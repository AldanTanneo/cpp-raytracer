#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include <cstdio>
#include <stdio.h>

// From src/include
#include <utils.hpp>

#ifdef _WIN32
    #define flockfile _lock_file
    #define funlockfile _unlock_file
#endif

/* Progress bar class */
class ProgressBar {
private:
    /* Clock used to measure total elapsed time */
    using clock = std::chrono::system_clock;
    /* Time point type */
    using time_point = std::chrono::time_point<clock>;
    /* Default bar subdivision */
    static constexpr size_t default_subdiv = 80;

    /* The current position of the progress bar */
    std::atomic<size_t> progress;
    /* The total size of the progress bar */
    const size_t size;
    /* Subdivisions of the progress bar */
    const size_t subdiv;
    /* The foreground character */
    const char * fchar;
    /* The background character */
    const char * bchar;
    /* Start time */
    const time_point start_time;

public:
    /* Construct a default progress bar with 80 subdivisions */
    inline ProgressBar(size_t size) noexcept
        : size(size), subdiv(default_subdiv), fchar("▓"), bchar("░"),
          start_time(clock::now()) {
        progress.store(0);
    }
    /* Constructs a progress bar, specify the number of subdivisions. */
    inline ProgressBar(size_t size, size_t subdiv) noexcept
        : size(size), subdiv(subdiv), fchar("▓"), bchar("░"),
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
        : size(size), subdiv(subdiv), fchar(fchar), bchar(bchar),
          start_time(clock::now()) {
        if (utils::utf8len(fchar) != utils::utf8len(bchar)) {
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
        size_t bchar_width = utils::utf8len(bchar);
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
    inline void advance(size_t n = 1) noexcept {
        const size_t prog = progress.fetch_add(n);
        const int draw =
            static_cast<int>(subdiv * static_cast<double>(prog + n) / size)
            - static_cast<int>(subdiv * static_cast<double>(prog) / size);
        if (draw) {
            flockfile(stdout);
            for (int i = 0; i < draw; ++i)
                fputs(fchar, stdout);
            fflush(stdout);
            funlockfile(stdout);
        }
    }

    inline void setback(size_t n = 1) noexcept {
        const size_t prog = progress.fetch_sub(n);
        const int draw =
            static_cast<int>(subdiv * static_cast<double>(prog) / size)
            - static_cast<int>(subdiv * static_cast<double>(prog - n) / size);
        if (draw) {
            const size_t bchar_w = utils::utf8len(bchar);
            flockfile(stdout);
            for (int i = 0; i < draw * bchar_w; ++i)
                fputc('\b', stdout);
            for (int i = 0; i < draw; ++i)
                fputs(bchar, stdout);
            for (int i = 0; i < draw * bchar_w; ++i)
                fputc('\b', stdout);
            fflush(stdout);
            funlockfile(stdout);
        }
    }

    /* Stop the progress bar, print a message ending with "in <elapsed time>" */
    inline void stop(const char * message = "Done") const noexcept {
        using namespace std::chrono;
        int64_t millis =
            duration_cast<milliseconds>(clock::now() - start_time).count();
        std::cout << term_colours::RESET << std::endl
                  << message << " in " << millis / 1000 << "s " << millis % 1000
                  << "ms" << std::endl;
        fflush(stdout);
    }

    /* Stop the progress bar, do not print a message */
    inline void stop_and_leave() const noexcept {
        puts(term_colours::RESET);
        fflush(stdout);
    }
};

#endif
