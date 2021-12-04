#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstdio>
#include <iostream>

// Common imports
#include <utils.hpp>

#ifndef _NO_ERROR_HANDLER_MACRO
    // Error handling macro that wraps main()
    #define main(...)                                                          \
        _main(int _argc, char * _argv[]);                                      \
        int main(int _argc, char * _argv[]) {                                  \
            int res = EXIT_SUCCESS;                                            \
            try {                                                              \
                res = _main(_argc, _argv);                                     \
            } catch (const char * s) {                                         \
                std::cout << "Uncaught exception: " << s << std::endl;         \
            } catch (const std::string & s) {                                  \
                std::cout << "Uncaught exception: " << s << std::endl;         \
            } catch (const std::exception & e) {                               \
                std::cout << "Uncaught exception: " << e.what() << std::endl;  \
            } catch (...) {                                                    \
                std::cout << "Uncaught exception: unknown exception"           \
                          << std::endl;                                        \
            }                                                                  \
            return res;                                                        \
        }                                                                      \
        int _main(int argc, char * argv[])
#endif

#endif
