#include <hittable.hpp>
#include <objects/sphere.hpp>
#include <utils/vec3.hpp>

bool Sphere::hit(const Ray & ray_in, double tmin, double tmax, HitRecord & hit_record) const {
    // sovling ( ray_origin + t * ray_direction - sphere_centre )^2 = sphere_radius^2
    double a = ray_in.direction.squared_norm();
    double b = 2.0 * ray_in.direction.dot(ray_in.origin - centre);
    double c = (ray_in.origin - centre).squared_norm() - radius * radius;
    double delta = b * b - 4.0 * a * c;
    if (delta < 0) { // sphere not hit
        return false;
    } else if (delta == 0) { // sphere hit in 1 point
        hit_record.time = -b / (2.0 * a);
        hit_record.hit_point = ray_in.at(hit_record.time);
        hit_record.surface_normal = (hit_record.hit_point - centre) / radius;
        return true;
    } else { // sphere hit in 2 points
        double t = (-b - sqrt(delta)) / (2.0 * a);
        t = (t > tmin) ? t : (-b + sqrt(delta)) / (2.0 * a); // smallest positive solution
        if (t > tmax) {
            return false;
        } else {
            hit_record.time = t;
            hit_record.hit_point = ray_in.at(hit_record.time);
            hit_record.surface_normal = (hit_record.hit_point - centre) / radius;
            return true;
        }
    }
}
