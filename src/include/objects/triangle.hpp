#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <type_traits>

// From src/include
#include <hittable.hpp>
#include <utils/vec3.hpp>

// Triangle class
class Triangle : public Hittable {
private:
    // First vertex of the triangle
    const Point3 vertex;
    // First edge of the triangle
    const Vec3 edge1;
    // Second edge of the triangle
    const Vec3 edge2;

    // Normal of the triangle
    const Vec3 normal;

    // Unit normal of the triangle
    const Vec3 unit_normal;

    // Material of the triangle
    const Material & material;

public:
    // Construct a triangle from its three vertices.
    // The order of the vertices defines the orientation of the triangle.
    template <class T>
    requires Material::is_material<T>
    inline Triangle(const Point3 & point1,
                    const Point3 & point2,
                    const Point3 & point3,
                    const T & material) noexcept
        : vertex(point1), edge1(point2 - point1), edge2(point3 - point1),
          normal((point2 - point1).cross(point3 - point1)),
          unit_normal((point2 - point1).cross(point3 - point1).unit_vector()),
          material(material) {}

    // Virtual function override
    virtual bool hit(const Ray & ray_in,
                     double tmin,
                     double tmax,
                     HitRecord & hit_record) const noexcept override;
};

#endif
