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
    // Default ONB: canonical base in R^3
    constexpr Onb() noexcept : u(vec3::X), v(vec3::Y), w(vec3::Z) {}

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

    // Construct an ONB from three vectors. Checks that they form a valid ONB
    inline static Onb
    from_base_vectors(const Vec3 u, const Vec3 v, const Vec3 w) {
        if (fabs(u.squared_norm() - 1) > utils::EPSILON
            || fabs(v.squared_norm() - 1) > utils::EPSILON
            || fabs(w.squared_norm() - 1) > utils::EPSILON
            || fabs(u.dot(v)) > utils::EPSILON
            || fabs(v.dot(w)) > utils::EPSILON
            || fabs(w.dot(u)) > utils::EPSILON
            || u.cross(v).distance(w) > utils::EPSILON
            || v.cross(w).distance(u) > utils::EPSILON
            || w.cross(u).distance(v) > utils::EPSILON) {
            throw "Not an orthonormal base!";
        }
        return Onb(u, v, w);
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
