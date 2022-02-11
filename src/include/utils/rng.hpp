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

    // Advance the random number generator by n * 2^32 state transitions
    void jump(uint64_t n) noexcept;

    // Return a random 32 bit integer using the high bits of the next 64 bit
    // integer
    inline uint32_t gen_u32() noexcept { return gen_u64() >> 32; }

    // Return a random double in the range [0, 1) using the high bits of the
    // next 64 bit integer
    inline double gen() noexcept {
        constexpr uint64_t _DOUBLE_SIZE = sizeof(double) * 8;
        constexpr uint64_t _DOUBLE_PRECISION = DBL_MANT_DIG;
        constexpr double _DOUBLE_SCALE =
            1.0 / double(1ULL << _DOUBLE_PRECISION);

        const uint64_t z = gen_u64() >> (_DOUBLE_SIZE - _DOUBLE_PRECISION);
        return _DOUBLE_SCALE * double(z);
    }

    // Return a random double in the range [a, b)
    inline double gen(const double a, const double b) noexcept {
        return a + gen() * (b - a);
    }

    // Return a random float in the range [0, 1) using the high bits of the next
    // 32 bit integer
    inline float gen_float() noexcept {
        constexpr uint64_t _FLOAT_SIZE = sizeof(float) * 8;
        constexpr uint64_t _FLOAT_PRECISION = FLT_MANT_DIG;
        constexpr double _FLOAT_SCALE = 1.0 / double(1ULL << _FLOAT_PRECISION);

        const uint32_t z = gen_u32() >> (_FLOAT_SIZE - _FLOAT_PRECISION);
        return _FLOAT_SCALE * double(z);
    }

    // Return a random float in the range [a, b)
    inline float gen_float(const float a, const float b) noexcept {
        return a + gen() * (b - a);
    }
} // namespace rng

#endif
