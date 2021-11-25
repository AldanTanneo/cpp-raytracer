#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>

/* Outputs a newline */
inline void newline() {
    printf("\n");
}

/* Debugging macro */
#define DBG(args)                                                                  \
    std::cout << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "] " \
              << #args << " = " << (args) << std::endl;

/* Error handling macro that wraps main() */
#define main(args)                                                             \
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
