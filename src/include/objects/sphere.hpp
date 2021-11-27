#ifndef SPHERE_HPP
#define SPHERE_HPP

// From src/include
#include <hittable.hpp>
#include <utils/vec3.hpp>

class Sphere : public Hittable {
  private:
    /* Centre of the sphere */
    Point3 centre;
    /* Radius of the sphere */
    double radius;
    /* Material of the sphere */
    std::shared_ptr<Material> material;

  public:
    /* Construct sphere from its centre and its radius */
    inline Sphere(Point3 centre, double radius, std::shared_ptr<Material> material) noexcept
        : centre(centre), radius(radius), material(material) {}

    /* define behaviour of ray when hitting sphere */
    virtual bool hit(const Ray & ray_in, double tmin, double tmax, HitRecord & hit_record) const noexcept override;
};

#endif
