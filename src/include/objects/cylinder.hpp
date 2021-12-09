#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include <cmath>
#include <type_traits>

// From src/include
#include <hittable.hpp>
#include <utils/vec3.hpp>

// Cylinder object
class Cylinder : public Hittable {
private:
    // Base point of the cylinder
    const Point3 base;
    // Axis of the cylinder
    const Vec3 axis;
    // Radius of the cylinder
    const double radius;
    // Height of the cylinder
    const double height;
    // Material of the cylinder
    const Material & material;

public:
    // Construct cylinder from its base, axis, radius and height
    template <class T>
    requires Material::is_material<T>
    inline Cylinder(const Point3 & base,
                    const Vec3 axis,
                    const double radius,
                    const double height,
                    const T & material) noexcept
        : base(base), axis(axis.unit_vector()), radius(radius), height(height),
          material(material) {}

    // Virtual function override
    virtual bool hit(const Ray & ray_in,
                     const double tmin,
                     const double tmax,
                     HitRecord & hit_record) const noexcept override;
};

#endif
