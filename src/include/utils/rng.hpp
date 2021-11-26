#ifndef RNG_HPP
#define RNG_HPP

#include <stdint.h>

/* Thread-local random number generator */
namespace rng {
/* Return a random double in the range [0, 1) */
double gen();
/* Return a random 64 bit integer */
uint64_t gen_u64();
/* Return a random 32 bit integer */
uint32_t gen_u32();
} // namespace rng

#endif
