#ifndef HITTABLE_HPP
#define HITTABLE_HPP

// From src/include
#include <ray.hpp>
#include <utils/vec3.hpp>

/* The returned structure if a ray hits an object */
struct HitRecord {
    /* The point of intersection between the ray and the object */
    Point3 hit_point;
    /* The normal of the object surface at the intersection point */
    Vec3 surface_normal;
    /* The material of the object surface */
    // Material mat;
};

class Hittable {
  public:
    // get_materiau

    /* define behaviour of ray when hitting hittable object */
    virtual bool hit(Ray ray_in, double tmin, double tmax, HitRecord & hit_record) const = 0;
};

#endif
