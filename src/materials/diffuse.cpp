#include <hittable.hpp>
#include <materials/diffuse.hpp>
#include <ray.hpp>
#include <utils/pdf.hpp>
#include <utils/vec3.hpp>

Lambertian::ScatterType
Lambertian::scatter(const HitRecord & hit_record,
                    const Ray & ray_in,
                    ScatterRecord & scatter) const noexcept {
    scatter.attenuation = albedo;
    scatter.is_specular = false;
    scatter.pdf = std::make_unique<CosinePdf>(hit_record.surface_normal);

    return ScatterType::Bounce;
}

double Lambertian::scattering_pdf(const HitRecord & hit_record,
                                  const Ray & scattered_ray) const noexcept {
    const double cos_theta =
        scattered_ray.direction.dot(hit_record.surface_normal);
    return cos_theta < utils::EPSILON ? 0.0 : cos_theta / utils::PI;
}

MicroFacet::ScatterType
MicroFacet::scatter(const HitRecord & hit_record,
                    const Ray & ray_in,
                    ScatterRecord & scatter) const noexcept {
    scatter.attenuation = albedo;
    scatter.is_specular = false;
    scatter.pdf = std::make_unique<OrenNayar>(
        hit_record.surface_normal, ray_in.direction.unit_vector(), A, B);

    return ScatterType::Bounce;
}

double MicroFacet::scattering_pdf(const HitRecord & hit_record,
                                  const Ray & scattered_ray) const noexcept {
    const double cos_theta =
        scattered_ray.direction.dot(hit_record.surface_normal);

    return cos_theta < utils::EPSILON ? 0.0 : cos_theta / utils::PI;
}
