#ifndef SPHERE_HPP
#define SPHERE_HPP

// From src/include
#include <hittable.hpp>
#include <utils/vec3.hpp>

class Sphere : public Hittable {
private:
    /* Centre of the sphere */
    const Point3 centre;
    /* Radius of the sphere */
    const double radius;
    /* Material of the sphere */
    const Material & material;

public:
    /* Construct sphere from its centre, radius and material */
    template <class T>
    constexpr Sphere(const Point3 & centre,
                     const double radius,
                     const T & material,
                     std::enable_if_t<std::is_convertible_v<T *, Material *>,
                                      void *> = nullptr) noexcept
        : centre(centre), radius(radius), material(material) {}

    /* Virtual function override */
    virtual bool hit(const Ray & ray_in,
                     double tmin,
                     double tmax,
                     HitRecord & hit_record) const noexcept override;
};

#endif
