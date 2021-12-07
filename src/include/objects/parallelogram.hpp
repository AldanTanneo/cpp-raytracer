#ifndef PARALLELOGRAM_HPP
#define PARALLELOGRAM_HPP

#include <type_traits>

// From src/include
#include <hittable.hpp>
#include <utils/vec3.hpp>

// Parallelogram class
class Parallelogram : public Hittable {
private:
    // First vertex of the parallelogram
    const Point3 vertex;
    // First edge of the parallelogram
    const Vec3 edge1;
    // Second edge of the parallelogram
    const Vec3 edge2;

    // Normal of the parallelogram
    const Vec3 normal;

    // Unit normal of the parallelogram
    const Vec3 unit_normal;

    // Material of the parallelogram
    const Material & material;

public:
    // Construct a parallelogram from its three defining vertices.
    // The fourth vertex is deduced.
    // The order of the vertices defines the orientation of the parallelogram.
    template <class T>
    requires Material::is_material<T>
    inline Parallelogram(const Point3 & point1,
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

    // Virtual function override
    virtual double pdf_value(const Point3 & origin,
                             const Vec3 & direction) const noexcept override {
        HitRecord rec;
        if (!this->hit(Ray(origin, direction), utils::EPSILON, utils::INF, rec))
            return 0.0;

        const double area = normal.dot(unit_normal);
        const double distance_squared =
            rec.time * rec.time * direction.squared_norm();
        const double cosine =
            fabs(direction.dot(rec.surface_normal) / direction.norm());

        return distance_squared / (cosine * area);
    }

    // Virtual function override
    virtual Vec3 random(const Point3 & origin) const noexcept override {
        return (vertex + rng::gen() * edge1 + rng::gen() * edge2 - origin)
            .unit_vector();
    }
};

#endif
