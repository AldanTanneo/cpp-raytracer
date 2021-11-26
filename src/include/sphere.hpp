#ifndef SPHERE_HPP
#define SPHERE_HPP

// From src/include
#include <hittable.hpp>

class Sphere : public Hittable {
  private:
    /* Centre of the sphere */
    Point3 centre;
    /* Radius of the sphere */
    double radius;

  public:
    /* Construct sphere from its centre and its radius */
    constexpr Sphere(Point3 centre, double radius) : centre(centre), radius(radius) {}

    /* define behaviour of ray when hitting sphere */
    virtual bool hit(Ray ray_in, double tmin, double tmax, Ray & ray_out) {
        // Vec3 normal 
    }
};

#endif