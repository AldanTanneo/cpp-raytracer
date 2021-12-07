#ifndef ORTHONORMAL_BASES_HPP
#define ORTHONORMAL_BASES_HPP

#include <cmath>

// From src/include
#include <utils/vec3.hpp>

// Orthonormal base
class Onb {
public:
    // Base vectors
    Vec3 u, v, w;

private:
    // Construct an ONB. Unsafe: does not check ONB validity
    constexpr Onb(const Vec3 u, const Vec3 v, const Vec3 w) noexcept
        : u(u), v(v), w(w) {}

public:
    // Construct an ONB from a normal vector
    inline static Onb from_unit_normal(const Vec3 & n) noexcept {
        const double x = fabs(n.x);
        const double y = fabs(n.y);
        const double z = fabs(n.z);
        const Vec3 a =
            x < y ? (x < z ? vec3::X : vec3::Z) : (y < z ? vec3::Y : vec3::Z);
        const Vec3 v = n.cross(a);
        return Onb(n.cross(v), v, n);
    }

    // Get the local expression of a vector, given its coordinates
    constexpr Vec3
    local(const double a, const double b, const double c) const noexcept {
        return a * u + b * v + c * w;
    }

    // Get the local expression of a vector
    constexpr Vec3 local(const Vec3 & a) const noexcept {
        return a.x * u + a.y * v + a.z * w;
    }
};

#endif
