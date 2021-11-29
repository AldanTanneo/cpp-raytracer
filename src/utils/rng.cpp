#include <cfloat>
#include <cstdint>

// From src/include
#include <float.h>
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
};

/* Global, thread-local RNG instance */
thread_local FastRng glob_rng(0x193a6754ULL);

/* Export a limited number of functions to the external library,
users cannot see the inside of the beast */
namespace rng {
    void seed(uint64_t seed) noexcept { glob_rng.value = seed; }

    uint64_t gen_u64() noexcept { return glob_rng.next_u64(); }
} // namespace rng
