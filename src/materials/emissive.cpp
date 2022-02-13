#include <hittable.hpp>
#include <materials/emissive.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

Emissive::ScatterType
    Emissive::scatter(const HitRecord & hit_record,
                      const Ray & ray_in,
                      ScatterRecord & scatter) const noexcept {
    scatter.attenuation =
        -ray_in.direction.unit_vector().dot(hit_record.surface_normal) * colour;
    return ScatterType::Emit;
}

BlackBody::ScatterType
    BlackBody::scatter(const HitRecord & hit_record,
                       const Ray & ray_in,
                       ScatterRecord & scatter) const noexcept {
    scatter.attenuation = hit_record.front_face * colour;
    return ScatterType::Emit;
}
