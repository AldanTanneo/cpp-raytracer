#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <type_traits>

// From src/include
#include <hittable.hpp>
#include <utils/vec3.hpp>

// Sphere object
class Sphere : public Hittable {
private:
    // Centre of the sphere
    const Point3 centre;
    // Radius of the sphere
    const double radius;
    // Material of the sphere
    const Material & material;

public:
    // Construct sphere from its centre, radius and material
    template <class T>
    requires Material::is_material<T>
    inline Sphere(const Point3 & centre,
                  const double radius,
                  const T & material) noexcept
        : centre(centre), radius(radius), material(material) {}

    // Virtual function override
    virtual bool hit(const Ray & ray_in,
                     const double tmin,
                     const double tmax,
                     HitRecord & hit_record) const noexcept override;
};

#endif
