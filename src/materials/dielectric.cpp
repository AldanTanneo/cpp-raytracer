#include <hittable.hpp>
#include <materials/dielectric.hpp>
#include <ray.hpp>
#include <utils.hpp>
#include <utils/rng.hpp>
#include <utils/vec3.hpp>

Dielectric::ScatterType
    Dielectric::scatter(const HitRecord & hit_record,
                        const Ray & ray_in,
                        ScatterRecord & scatter) const noexcept {
    scatter.attenuation = colour;
    scatter.is_specular = true;

    const double r2 = ray_in.direction.squared_norm();
    const double r_cos_theta = -ray_in.direction.dot(hit_record.surface_normal);
    const double cos_theta = fmin(r_cos_theta / sqrt(r2), 1.0);
    const double sin_theta = sqrt(1 - cos_theta * cos_theta);
    const double refraction_ratio =
        (hit_record.front_face) ? 1 / refraction_index : refraction_index;

    if (refraction_ratio * sin_theta > 1
        || utils::reflectance(cos_theta, refraction_ratio) > rng::gen()) {
        // Reflexion
        scatter.specular_direction =
            ray_in.direction + 2.0 * r_cos_theta * hit_record.surface_normal;
    } else {
        // Refraction
        Vec3 orth_out =
            refraction_ratio
            * (ray_in.direction + r_cos_theta * hit_record.surface_normal);
        Vec3 parr_out =
            -sqrt(r2 - orth_out.squared_norm()) * hit_record.surface_normal;
        scatter.specular_direction = orth_out + parr_out;
    }
    return ScatterType::Bounce;
}

Translucent::ScatterType
    Translucent::scatter(const HitRecord & hit_record,
                         const Ray & ray_in,
                         ScatterRecord & scatter) const noexcept {
    scatter.attenuation = colour;
    scatter.is_specular = true;

    const double r2 = ray_in.direction.squared_norm();
    const double r_cos_theta = -ray_in.direction.dot(hit_record.surface_normal);
    const double cos_theta = fmin(r_cos_theta / sqrt(r2), 1.0);
    const double sin_theta = sqrt(1 - cos_theta * cos_theta);
    const double refraction_ratio =
        (hit_record.front_face) ? 1 / refraction_index : refraction_index;

    if (refraction_ratio * sin_theta > 1
        || utils::reflectance(cos_theta, refraction_ratio) > rng::gen()) {
        // Reflexion
        scatter.specular_direction =
            ray_in.direction + 2.0 * r_cos_theta * hit_record.surface_normal;
    } else {
        // Refraction
        Vec3 orth_out =
            refraction_ratio
            * (ray_in.direction + r_cos_theta * hit_record.surface_normal);
        double parralel_length = sqrt(r2 - orth_out.squared_norm());
        Vec3 parr_out = -parralel_length * hit_record.surface_normal;
        scatter.specular_direction =
            orth_out + parr_out
            + parralel_length * Vec3::random_in_unit_sphere();
    }
    return ScatterType::Bounce;
}
