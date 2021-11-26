#include <hittable.hpp>
#include <objects/sphere.hpp>
#include <utils/vec3.hpp>

bool Sphere::hit(const Ray & ray_in, double tmin, double tmax, HitRecord & hit_record) const noexcept {
    // sovling ( ray_origin + t * ray_direction - sphere_centre )^2 = sphere_radius^2
    double a = ray_in.direction.squared_norm();
    double b = 2.0 * ray_in.direction.dot(ray_in.origin - centre);
    double c = (ray_in.origin - centre).squared_norm() - radius * radius;
    double delta = b * b - 4.0 * a * c;

    double time = tmin;

    if (delta == 0) { // sphere hit in 1 point
        time = -b / (2.0 * a);
    } else { // sphere hit in 2 points
        time = (-b - sqrt(delta)) / (2.0 * a);
        time = (time > tmin) ? time : (-b + sqrt(delta)) / (2.0 * a); // smallest positive solution
    }

    if (tmin < tmax && time < tmax) {
        hit_record.time = time;
        hit_record.hit_point = ray_in.at(time);
        hit_record.surface_normal = (hit_record.hit_point - centre) / radius;
        return true;
    } else {
        return false;
    }
}
