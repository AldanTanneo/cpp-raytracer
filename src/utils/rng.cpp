#include <stdint.h>
#include <utils/rng.hpp>

/* Private random number generator based on SplitMix64. */
struct FastRng {
    /* The inner 64-bit value */
    uint64_t value;

    /* Construct a new instance from a seed */
    constexpr FastRng(const uint64_t seed) noexcept : value(seed) {}

    /* Fetch the next 64-bit integer */
    constexpr uint64_t next_u64() noexcept {
        uint64_t z = value + 0x9E3779B97F4A7C15ULL;
        value = z;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }

    /* Fetch the next 64-bit integer and cast it to a double in the [0, 1) range */
    inline double next_f64() noexcept {
        uint64_t i = next_u64();
        constexpr uint64_t mask1 = 0x3FF0000000000000ULL;
        constexpr uint64_t mask2 = 0x3FFFFFFFFFFFFFFFULL;
        const uint64_t to_12 = (i | mask1) & mask2;
        return reinterpret_cast<const double &>(to_12) - 1;
    }
};

/* Global, thread-local RNG instance */
thread_local FastRng glob_rng(0x193a6754ULL);

/* Export a limited number of functions to the external library,
users cannot see the inside of the beast */
namespace rng {
double gen() {
    return glob_rng.next_f64();
}

uint64_t gen_u64() {
    return glob_rng.next_u64();
}

uint32_t gen_u32() {
    return glob_rng.next_u64() >> 32;
}
} // namespace rng
