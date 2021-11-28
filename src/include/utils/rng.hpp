#ifndef RNG_HPP
#define RNG_HPP

#include <stdint.h>

/* Thread-local random number generator */
namespace rng {
    /* Seed the RNG using a 64 bits integer */
    void seed(uint64_t seed) noexcept;
    /* Return a random double in the range [0, 1) */
    double gen() noexcept;
    /* Return a random 64 bit integer */
    uint64_t gen_u64() noexcept;
    /* Return a random 32 bit integer */
    uint32_t gen_u32() noexcept;
} // namespace rng

#endif
