#include "utils/rng.hpp"

#include <hittable.hpp>
#include <materials/dielectric.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

bool Dielectric::scatter(const HitRecord & hit_record,
                         Ray & ray,
                         Colour & ray_colour) const noexcept {
    ray.origin = hit_record.hit_point;
    double r = ray.direction.norm();
    double cos_theta = -ray.direction.dot(hit_record.surface_normal) / r;
    double sin_theta = sqrt(1 - cos_theta * cos_theta);
    double refraction_ratio =
        (hit_record.front_face) ? 1 / refraction_index : refraction_index;

    if (refraction_ratio * sin_theta > 1
        || reflectance(cos_theta, refraction_ratio) > rng::gen()) {
        // Reflexion
        ray.direction -= 2.0 * r * cos_theta * hit_record.surface_normal;
    } else { // Refraction
        ray.direction =
            ray.direction.refract(hit_record.surface_normal, refraction_ratio);
    }
    ray_colour *= colour;
    return true;
}
