#include <hittable.hpp>
#include <materials/plastic.hpp>
#include <ray.hpp>
#include <utils.hpp>
#include <utils/rng.hpp>
#include <utils/vec3.hpp>

Plastic::ScatterType Plastic::scatter(const HitRecord & hit_record,
                                      Ray & ray,
                                      Colour & ray_colour) const noexcept {
    ray.origin = hit_record.hit_point;
    const double r_cos_theta = -ray.direction.dot(hit_record.surface_normal);
    const double cos_theta = fmin(r_cos_theta / ray.direction.norm(), 1.0);

    if (utils::reflectance(cos_theta, 0.67) > rng::gen()) {
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
