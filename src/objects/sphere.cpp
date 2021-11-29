// From src/include
#include <hittable.hpp>
#include <objects/sphere.hpp>
#include <utils/vec3.hpp>

bool Sphere::hit(const Ray & ray_in,
                 double tmin,
                 double tmax,
                 HitRecord & hit_record) const noexcept {
    // solving (origin + t * dir - centre)^2 = R^2
    double a = ray_in.direction.squared_norm();
    double b = 2.0 * ray_in.direction.dot(ray_in.origin - centre);
    double c = (ray_in.origin - centre).squared_norm() - radius * radius;
    double delta = b * b - 4.0 * a * c;

    double time = tmin;

    if (delta == 0) { // sphere hit in 1 point
        time = -b / (2.0 * a);
    } else { // sphere hit in 2 points
        time = (-b - sqrt(delta)) / (2.0 * a);
        if (time < tmin) {
            time = (-b + sqrt(delta)) / (2.0 * a); // smallest positive solution
        }
    }

    if (tmin < time && time < tmax) {
        // There was a hit in the time window
        hit_record.time = time;
        hit_record.hit_point = ray_in.at(time);
        Vec3 outward_normal = (hit_record.hit_point - centre) / radius;
        hit_record.set_face_normal(ray_in, outward_normal);
        hit_record.material = material;
        return true;
    } else {
        // No hit between tmin and tmax
        return false;
    }
}
