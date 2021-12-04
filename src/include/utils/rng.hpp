#ifndef RNG_HPP
#define RNG_HPP

#include <cfloat>
#include <cstdint>

// Thread-local random number generator
namespace rng {
    // Seed the RNG using a 64 bits integer
    void seed(uint64_t seed) noexcept;

    // Return a random 64 bit integer
    uint64_t gen_u64() noexcept;

    // Return a random 32 bit integer using the high bits of the next 64 bit
    // integer
    inline uint32_t gen_u32() noexcept { return gen_u64() >> 32; }

    constexpr uint64_t _DOUBLE_SIZE = sizeof(double) * 8;
    constexpr uint64_t _DOUBLE_PRECISION = DBL_MANT_DIG;
    constexpr double _SCALE = 1.0 / double(1ULL << _DOUBLE_PRECISION);

    // Return a random double in the range [0, 1) using the high bits of the
    // next 64 bit integer
    inline double gen() noexcept {
        const uint64_t z = gen_u64() >> (_DOUBLE_SIZE - _DOUBLE_PRECISION);
        return _SCALE * double(z);
    }

    // Return a random double in the range [a, b)
    inline double gen(const double a, const double b) noexcept {
        return a + gen() * (b - a);
    }
} // namespace rng

#endif
