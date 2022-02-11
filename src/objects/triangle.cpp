#include <cmath>

// From src/include
#include <hittable.hpp>
#include <objects/triangle.hpp>
#include <utils/vec3.hpp>

bool Triangle::hit(const Ray & ray,
                   const double tmin,
                   const double tmax,
                   HitRecord & hit_record) const noexcept {
    // Test if the ray's direction is colinear to the triangle
    const double determinant = ray.direction.dot(normal);
    if (fabs(determinant) < utils::EPSILON) {
        return false;
    }
    const double scale = 1.0 / determinant;
    const Point3 origin_to_vertex = vertex - ray.origin;

    // Finding the time of intersection
    const double time = scale * origin_to_vertex.dot(normal);
    if (time < tmin || tmax < time) {
        return false;
    }

    // Test if the intersection point is in the triangle (using barycentric
    // coordinates lambda, mu)
    const Vec3 direction_cross_origin_to_vertex =
        ray.direction.cross(origin_to_vertex);

    const double lambda = scale * direction_cross_origin_to_vertex.dot(edge1);
    if (lambda < 0.0 || lambda > 1.0) {
        return false;
    }

    const double mu = -scale * direction_cross_origin_to_vertex.dot(edge2);
    if (mu < 0.0 || lambda + mu > 1.0) {
        return false;
    }

    hit_record.time = time;
    hit_record.hit_point = ray.at(time);
    Vec3 outward_normal = unit_normal;
    hit_record.set_face_normal(ray, outward_normal);
    hit_record.material = material;
    return true;
}

double Triangle::pdf_value(const Point3 & origin,
                           const Vec3 & direction) const noexcept {
    HitRecord rec;
    if (!this->hit(Ray(origin, direction), utils::EPSILON, utils::INF, rec)) {
        return 0.0;
    }

    const double area = 0.5 * normal.dot(unit_normal);
    const double distance_squared =
        rec.time * rec.time * direction.squared_norm();
    const double cosine =
        fabs(direction.dot(rec.surface_normal) / direction.norm());

    return distance_squared / (cosine * area);
}

Vec3 Triangle::random(const Point3 & origin) const noexcept {
    double alpha = rng::gen();
    double beta = rng::gen() * alpha;
    return (vertex + alpha * edge1 + beta * edge2 - origin).unit_vector();
}
