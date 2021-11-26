#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstdio>
#include <iostream>

/* Outputs a newline */
inline void newline() {
    printf("\n");
}

constexpr char _esc = 27;
constexpr char _cyan[6] = {_esc, '[', '3', '6', 'm', '\0'};
constexpr char _nc[5] = {_esc, '[', '0', 'm', '\0'};
constexpr char _bold[5] = {_esc, '[', '1', 'm', '\0'};

#define _DBG_COUT_IDENT "[" __FILE__ ":" << __FUNCTION__ << ":" << __LINE__ << "] "

#define STRINGIFY(args) #args

#ifdef _DISABLE_DEBUGGING
#define DEBUG(...) ;
#else
/* Debugging macro */
#define DEBUG(args, ...)                                               \
    __VA_OPT__(std::cout << _cyan << _bold << _DBG_COUT_IDENT << _nc   \
                         << _bold << __VA_ARGS__ << _nc << std::endl;) \
    std::cout << _cyan << _bold << _DBG_COUT_IDENT << _nc              \
              << (#args) << " = " << (args) << std::endl;
#endif

/* Error handling macro that wraps main() */
#define main(...)                                                              \
    _main(int _argc, char * _argv[]);                                          \
    int main(int _argc, char * _argv[]) {                                      \
        int res = EXIT_SUCCESS;                                                \
        try {                                                                  \
            res = _main(_argc, _argv);                                         \
        } catch (const char * s) {                                             \
            std::cout << "Uncaught exception: " << s << std::endl;             \
        } catch (const std::string & s) {                                      \
            std::cout << "Uncaught exception: " << s << std::endl;             \
        } catch (const std::exception & e) {                                   \
            std::cout << "Uncaught exception: " << e.what() << std::endl;      \
        } catch (...) {                                                        \
            std::cout << "Uncaught exception: unknown exception" << std::endl; \
        }                                                                      \
        return res;                                                            \
    }                                                                          \
    int _main(int argc, char * argv[])

#endif
