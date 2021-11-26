#ifndef HITTABLE_HPP
#define HITTABLE_HPP

// From src/include
#include <ray.hpp>

class Hittable {
  public:
    //get_materiau

    /* define behaviour of ray when hitting hittable object */
    virtual bool hit(Ray ray_in, double tmin, double tmax, Ray& ray_out) = 0;
};

#endif