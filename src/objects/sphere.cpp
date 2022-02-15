// From src/include
#include <hittable.hpp>
#include <objects/sphere.hpp>
#include <utils/vec3.hpp>

bool Sphere::hit(const Ray & ray,
                 const double tmin,
                 const double tmax,
                 HitRecord & hit_record) const noexcept {
    // solving (origin + t * dir - centre)^2 = R^2
    const double a = ray.direction.squared_norm();
    const double b = ray.direction.dot(ray.origin - centre);
    const double c = (ray.origin - centre).squared_norm() - radius * radius;
    const double delta = b * b - a * c;

    double time = tmin - utils::EPSILON;

    if (delta >= 0) { // sphere hit
        const double sqrt_delta = sqrt(delta);
        time = (-b - sqrt_delta) / a;

        if (time <= tmin) {
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

double Sphere::pdf_value(const Point3 & origin,
                         const Vec3 & direction) const noexcept {
    HitRecord rec;
    if (!this->hit(Ray(origin, direction), utils::EPSILON, utils::INF, rec)) {
        return 0;
    }

    double cos_theta_max =
        sqrt(1.0 - radius * radius / (centre - origin).squared_norm());
    double solid_angle = utils::TAU * (1.0 - cos_theta_max);

    return 1.0 / solid_angle;
}

static inline Vec3 random_to_sphere(double radius,
                                    double distance_squared) noexcept {
    double r1 = rng::gen();
    double r2 = rng::gen();
    double z =
        1.0 + r2 * (sqrt(1.0 - radius * radius / distance_squared) - 1.0);

    double phi = utils::TAU * r1;
    double x = cos(phi) * sqrt(1 - z * z);
    double y = sin(phi) * sqrt(1 - z * z);

    return Vec3(x, y, z);
}

Vec3 Sphere::random(const Point3 & origin) const noexcept {
    Vec3 direction = centre - origin;
    double distance_squared = direction.squared_norm();
    Onb uvw;
    uvw.from_unit_normal(direction / sqrt(distance_squared));
    return uvw.local(random_to_sphere(radius, distance_squared));
}
