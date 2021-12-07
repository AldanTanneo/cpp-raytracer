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
                                      const Ray & ray_in,
                                      ScatterRecord & scatter) const noexcept {
    scatter.attenuation = colour;
    const double rcos_theta = -ray_in.direction.dot(hit_record.surface_normal);
    const double cos_theta = rcos_theta / ray_in.direction.norm();

    if (plastic_reflectance(cos_theta) > rng::gen()) {
        // Reflexion
        scatter.is_specular = true;
        scatter.specular_direction =
            ray_in.direction
            + rcos_theta
                  * (2.0 * (hit_record.surface_normal)
                     + roughness * Vec3::random_in_unit_sphere());
    } else {
        // Diffusion
        scatter.is_specular = false;
        scatter.pdf =
            std::make_unique<CosinePdf>(CosinePdf(hit_record.surface_normal));
    }

    return ScatterType::Bounce;
}

double Plastic::scattering_pdf(const HitRecord & hit_record,
                               const Ray & scattered_ray) const noexcept {
    const double cos_theta =
        scattered_ray.direction.unit_vector().dot(hit_record.surface_normal);
    return cos_theta < utils::EPSILON ? 0.0 : cos_theta / utils::PI;
}
