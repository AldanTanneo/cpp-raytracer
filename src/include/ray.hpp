#ifndef RAY_HPP
#define RAY_HPP

// From src/include
#include <vec3.hpp>

class Ray {
  private:
    Point3 origin;
    Vec3 direction;

  public:
    constexpr Ray(Point3 origin, Vec3 direction)
        : origin(origin), direction(direction) {}

    Point3 at(double t0) { return origin + t0 * direction; }
};

#endif