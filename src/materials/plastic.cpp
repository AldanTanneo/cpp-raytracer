#include <hittable.hpp>
#include <materials/plastic.hpp>
#include <ray.hpp>
#include <utils.hpp>
#include <utils/rng.hpp>
#include <utils/vec3.hpp>

constexpr double plastic_refraction_index = 1.5;
constexpr double refraction_ratio = 1.0 / plastic_refraction_index;
constexpr double sr0 = (1.0 - refraction_ratio) / (1.0 + refraction_ratio);
constexpr double r0 = sr0 * sr0;

constexpr double plastic_reflectance(const double cos_theta) noexcept {
    const double one_minus_cos = 1.0 - cos_theta;
    const double squared = one_minus_cos * one_minus_cos;
    return r0 + (1.0 - r0) * squared * squared * one_minus_cos;
}

Plastic::ScatterType Plastic::scatter(const HitRecord & hit_record,
                                      Ray & ray,
                                      Colour & ray_colour) const noexcept {
    ray.origin = hit_record.hit_point;
    const double r_cos_theta = -ray.direction.dot(hit_record.surface_normal);
    const double cos_theta = fmin(r_cos_theta / ray.direction.norm(), 1.0);

    if (plastic_reflectance(cos_theta) > rng::gen()) {
        // Reflexion
        ray.direction =
            ray.direction
            + r_cos_theta
                  * (roughness * Vec3::random_in_unit_sphere()
                     - 2.0 * r_cos_theta * hit_record.surface_normal);
    } else {
        // Diffusion
        ray.direction =
            hit_record.surface_normal + Vec3::random_in_unit_sphere();
        if (ray.direction.near_zero()) {
            ray.direction = hit_record.surface_normal;
        }
    }
    ray_colour *= colour;
    return ScatterType::Bounce;
}
