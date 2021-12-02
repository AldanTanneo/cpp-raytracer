// From src/include
#include <hittable.hpp>
#include <objects/cylinder.hpp>
#include <utils/vec3.hpp>

bool Cylinder::hit(const Ray & ray,
                   double tmin,
                   double tmax,
                   HitRecord & hit_record) const noexcept {
    const double origin_minus_base_dot_axis = (ray.origin - base).dot(axis);
    const Point3 proj_origin_minus_base =
        ray.origin - base - origin_minus_base_dot_axis * axis;
    const double direction_dot_axis = ray.direction.dot(axis);
    const Vec3 proj_dir = ray.direction - direction_dot_axis * axis;
    const double a = proj_dir.squared_norm();
    const double b = proj_origin_minus_base.dot(proj_dir);
    const double c = proj_origin_minus_base.squared_norm() - radius * radius;
    const double delta = b * b - a * c;

    double time = tmin - utils::EPSILON;
    double lambda = -utils::EPSILON;
    double old_lambda = height / 2;
    if (delta >= 0) {
        const double sqrt_delta = sqrt(delta);
        time = (-b - sqrt_delta) / a;
        lambda = origin_minus_base_dot_axis + time * direction_dot_axis;

        if (lambda < 0 || height < lambda || time < tmin || tmax < time) {
            time = (-b + sqrt_delta) / a;
            /* Store the old lambda value, for disk checks */
            old_lambda = lambda;
            lambda = origin_minus_base_dot_axis + time * direction_dot_axis;
        }
    }

    Vec3 outward_normal = (ray.at(time) - (base + lambda * axis)) / radius;
    if (tmin < time && time < tmax && 0 <= lambda && lambda <= height) {
        if (old_lambda > height) {
            outward_normal = axis;
            time = (height - origin_minus_base_dot_axis) / direction_dot_axis;
            lambda = height;
        }
        if (old_lambda < 0) {
            outward_normal = -axis;
            time = -origin_minus_base_dot_axis / direction_dot_axis;
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
