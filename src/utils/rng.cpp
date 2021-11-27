#include <cfloat>
#include <cstdint>

// From src/include
#include <float.h>
#include <utils/rng.hpp>

constexpr uint64_t DOUBLE_SIZE = sizeof(double) * 8;
constexpr uint64_t PRECISION = DBL_MANT_DIG;
constexpr double SCALE = 1.0 / static_cast<double>(1ULL << PRECISION);

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
    constexpr double next_f64() noexcept {
        const uint64_t z = next_u64() >> (DOUBLE_SIZE - PRECISION);
        return SCALE * static_cast<double>(z);
    }
};

/* Global, thread-local RNG instance */
thread_local FastRng glob_rng(0x193a6754ULL);

/* Export a limited number of functions to the external library,
users cannot see the inside of the beast */
namespace rng {
void seed(uint64_t seed) noexcept {
    glob_rng.value = seed;
}

double gen() noexcept {
    return glob_rng.next_f64();
}

uint64_t gen_u64() noexcept {
    return glob_rng.next_u64();
}

uint32_t gen_u32() noexcept {
    return glob_rng.next_u64() >> 32;
}
} // namespace rng
