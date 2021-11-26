#include <stdint.h>
#include <utils/rng.hpp>

struct FastRng {
    uint64_t value;

    constexpr FastRng(const uint64_t seed) noexcept : value(seed) {}

    constexpr uint64_t next_u64() noexcept {
        uint64_t z = value + 0x9E3779B97F4A7C15ULL;
        value = z;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }

    inline double next_f64() noexcept {
        uint64_t i = next_u64();
        constexpr uint64_t mask1 = 0x3FF0000000000000ULL;
        constexpr uint64_t mask2 = 0x3FFFFFFFFFFFFFFFULL;
        const uint64_t to_12 = (i | mask1) & mask2;
        return reinterpret_cast<const double &>(to_12) - 1;
    }
};

thread_local FastRng glob_rng(0x193a6754ULL);

namespace rng {
double gen() {
    return glob_rng.next_f64();
}
} // namespace rng
