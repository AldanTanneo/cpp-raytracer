#ifndef RAY_HPP
#define RAY_HPP

// From src/include
#include <utils/vec3.hpp>

/* Light ray class with origin and direction */
class Ray {
  private:
    /* The origin of the ray */
    Point3 origin;
    /* The direction of the ray */
    Vec3 direction;

  public:
    /* Construct a ray from its origin and direction */
    constexpr Ray(Point3 origin, Vec3 direction) noexcept : origin(origin), direction(direction) {}

    /* Compute the position of the ray at time t0 */
    constexpr Point3 at(double t0) noexcept { return origin + t0 * direction; }

    /* Print the ray for debugging purposes */
    template <class charT, class charTraits = std::char_traits<charT>>
    inline friend std::basic_ostream<charT, charTraits> & operator<<(std::basic_ostream<charT, charTraits> & os, const Ray & self) {
        return os << "Ray(" << self.origin << ", " << self.direction << ")";
    }
};

#endif
