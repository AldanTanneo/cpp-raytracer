// From src/include
#include <hittable.hpp>
#include <objects/sphere.hpp>
#include <utils/vec3.hpp>

bool Sphere::hit(const Ray & ray,
                 double tmin,
                 double tmax,
                 HitRecord & hit_record) const noexcept {
    // solving (origin + t * dir - centre)^2 = R^2
    const double a = ray.direction.squared_norm();
    const double b = ray.direction.dot(ray.origin - centre);
    const double c = (ray.origin - centre).squared_norm() - radius * radius;
    const double delta = b * b - a * c;

    double time = tmin;

    if (delta >= 0) { // sphere hit
        const double sqrt_delta = sqrt(delta);
        time = (-b - sqrt_delta) / a;

        if (time < tmin) {
            time = (-b + sqrt_delta) / a; // smallest positive solution
        }
    }

    if (tmin < time && time < tmax) {
        // There was a hit in the time window
        hit_record.time = time;
        hit_record.hit_point = ray.at(time);
        Vec3 outward_normal = (hit_record.hit_point - centre) / radius;
        hit_record.set_face_normal(ray, outward_normal);
        hit_record.material = material;
        return true;
    } else {
        // No hit between tmin and tmax
        return false;
    }
}
