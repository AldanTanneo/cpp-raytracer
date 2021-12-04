#include <cmath>

// From src/include
#include <hittable.hpp>
#include <objects/triangle.hpp>
#include <utils/vec3.hpp>

bool Triangle::hit(const Ray & ray,
                   double tmin,
                   double tmax,
                   HitRecord & hit_record) const noexcept {
    // Test if ray's direction is (almost) colinear to the triangle ?
    const double delta = ray.direction.dot(normal);
    if (fabs(delta) < utils::EPSILON) {
        return false;
    }

    // Finding the moment of impact
    double time = (vertex - ray.origin).dot(normal) / delta;

    // Test if is negative (behind the camera) or not in the time window
    if (time < tmin || time > tmax) {
        return false;
    }

    // Test if I is in the triangle (using barycentric coordinates w1, w2)
    const double determinant = edge2.cross(edge1).z;
    const Point3 intersection_point = ray.at(time);
    const Vec3 vertex_to_intersection = intersection_point - vertex;

    const double lambda = vertex_to_intersection.cross(edge1).z / determinant;
    if (lambda < 0) {
        return false;
    }

    const double mu = -vertex_to_intersection.cross(edge2).z / determinant;
    if (mu < 0 || lambda + mu > 1) {
        return false;
    }

    // Else, there was a hit in the time window
    hit_record.time = time;
    hit_record.hit_point = intersection_point;
    Vec3 outward_normal = normal;
    hit_record.set_face_normal(ray, outward_normal);
    hit_record.material = material;
    return true;
}
