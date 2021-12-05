#include <hittable.hpp>
#include <materials/plastic.hpp>
#include <ray.hpp>
#include <utils.hpp>
#include <utils/rng.hpp>
#include <utils/vec3.hpp>

constexpr double plastic_reflectance(const double cos_theta) noexcept {
    return 0.04 + 0.96 * pow(1.0 - cos_theta, 5);
}

Plastic::ScatterType Plastic::scatter(const HitRecord & hit_record,
                                      Ray & ray,
                                      Colour & ray_colour) const noexcept {
    const double rcos_theta = -ray.direction.dot(hit_record.surface_normal);
    const double cos_theta = rcos_theta / ray.direction.norm();

    if (plastic_reflectance(cos_theta) > rng::gen()) {
        // Reflexion
        ray.direction += rcos_theta
                         * (2.0 * (hit_record.surface_normal)
                            + roughness * Vec3::random_in_unit_sphere());
    } else {
        // Diffusion
        ray.direction =
            hit_record.surface_normal + Vec3::random_in_unit_sphere();
        if (ray.direction.near_zero()) {
            ray.direction = hit_record.surface_normal;
        }
    }
    ray.origin = hit_record.hit_point;
    ray_colour *= colour;
    return ScatterType::Bounce;
}
