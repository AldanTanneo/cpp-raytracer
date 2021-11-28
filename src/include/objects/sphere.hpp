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
    std::unique_ptr<Material> material;

public:
    /* Construct sphere from its centre and its radius */
    template <class T>
    inline Sphere(const Point3 & centre,
                  const double radius,
                  const T material,
                  std::enable_if_t<std::is_convertible_v<T *, Material *>,
                                   void *> = nullptr) noexcept
        : centre(centre), radius(radius),
          material(std::make_unique<T>(material)) {}

    inline Sphere(Sphere & other) noexcept
        : centre(other.centre), radius(other.radius),
          material(std::move(other.material)) {}

    /* define behaviour of ray when hitting sphere */
    virtual bool hit(const Ray & ray_in,
                     double tmin,
                     double tmax,
                     HitRecord & hit_record) const noexcept override;
};

#endif
