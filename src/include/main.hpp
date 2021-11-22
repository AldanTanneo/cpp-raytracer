#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>

/* Error handling macro that wraps main() */
#define main()                                                                 \
    _main();                                                                   \
    int main() {                                                               \
        int res = EXIT_SUCCESS;                                                \
        try {                                                                  \
            res = _main();                                                     \
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
    int _main()

#endif
