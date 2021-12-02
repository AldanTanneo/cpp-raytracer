// From src/include
#include <hittable.hpp>
#include <objects/cylinder.hpp>
#include <utils/vec3.hpp>

bool Cylinder::hit(const Ray & ray,
                   double tmin,
                   double tmax,
                   HitRecord & hit_record) const noexcept {
    // Project the ray on the cylinder base plane to get possible intersections
    const double origin_minus_base_dot_axis = (ray.origin - base).dot(axis);
    const Point3 proj_origin_minus_base =
        ray.origin - base - origin_minus_base_dot_axis * axis;
    const double direction_dot_axis = ray.direction.dot(axis);
    const Vec3 proj_dir = ray.direction - direction_dot_axis * axis;
    // Solve (origin + t * direction - base)^2 = r^2, projected in the cylinder
    // base plane
    const double a = proj_dir.squared_norm();
    const double b = proj_origin_minus_base.dot(proj_dir);
    const double c = proj_origin_minus_base.squared_norm() - radius * radius;
    const double delta = b * b - a * c;

    // Time of intersection
    double time = tmin - utils::EPSILON;
    // Position of the intersection along the axis
    double lambda = -utils::EPSILON;
    double old_lambda = height / 2;
    if (delta >= 0) {
        const double sqrt_delta = sqrt(delta);
        time = (-b - sqrt_delta) / a;
        lambda = origin_minus_base_dot_axis + time * direction_dot_axis;

        if (lambda < 0 || height < lambda || time <= tmin) {
            time = (-b + sqrt_delta) / a;
            // Store the old lambda value, for disk checks
            old_lambda = lambda;
            lambda = origin_minus_base_dot_axis + time * direction_dot_axis;
        }
    }

    Vec3 outward_normal = (ray.at(time) - (base + lambda * axis)) / radius;
    // If the hit is inside the cylinder but the first possible hit was above or
    // below it, a disk was hit
    if (lambda <= height && old_lambda > height) {
        // Upper disk
        const double t0 =
            (height - origin_minus_base_dot_axis) / direction_dot_axis;
        if (tmin < t0) {
            time = t0;
            outward_normal = axis;
            lambda = height;
        }
    }
    if (0 <= lambda && old_lambda < 0) {
        // Lower disk
        const double t0 = -origin_minus_base_dot_axis / direction_dot_axis;
        if (tmin < t0) {
            time = t0;
            outward_normal = -axis;
            lambda = 0;
        }
    }

    if (tmin < time && time < tmax && 0 <= lambda && lambda <= height) {
        hit_record.time = time;
        hit_record.hit_point = ray.at(time);
        hit_record.set_face_normal(ray, outward_normal);
        hit_record.material = material;
        return true;
    }

    return false;
}
