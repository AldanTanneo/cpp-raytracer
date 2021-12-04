#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <type_traits>

// From src/include
#include <hittable.hpp>
#include <utils/vec3.hpp>

class Triangle : public Hittable {
private:
    const Point3 vertex;
    const Vec3 edge1;
    const Vec3 edge2;

    /* normal */
    const Vec3 normal;

    /* Material of the triangle */
    const Material & material;


public:
    /* Construct triangle from its three vertices */
    template <class T>
    constexpr Triangle(const Point3 & point1,
                     const Point3 & point2,
                     const Point3 & point3,
                     const T & material,
                     std::enable_if_t<std::is_convertible_v<T *, Material *>,
                                      void *> = nullptr) noexcept
        : vertex(point1), edge1(point2-point1), edge2(point3-point1), normal((point2-point1).cross(point3-point1).unit_vector()), material(material) {}

    /* Virtual function override */
    virtual bool hit(const Ray & ray_in,
                     double tmin,
                     double tmax,
                     HitRecord & hit_record) const noexcept override;
};

#endif
