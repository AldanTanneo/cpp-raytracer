#include <hittable.hpp>
#include <materials/diffuse.hpp>
#include <ray.hpp>
#include <utils/pdf.hpp>
#include <utils/vec3.hpp>

Diffuse::ScatterType Diffuse::scatter(const HitRecord & hit_record,
                                      const Ray & ray_in,
                                      ScatterRecord & scatter) const noexcept {
    scatter.attenuation = albedo;
    scatter.is_specular = false;
    scatter.pdf =
        std::make_unique<CosinePdf>(CosinePdf(hit_record.surface_normal));

    return ScatterType::Bounce;
}

double Diffuse::scattering_pdf(const HitRecord & hit_record,
                               const Ray & scattered_ray) const noexcept {
    const double cos_theta =
        scattered_ray.direction.unit_vector().dot(hit_record.surface_normal);
    return cos_theta < utils::EPSILON ? 0.0 : cos_theta / utils::PI;
}
