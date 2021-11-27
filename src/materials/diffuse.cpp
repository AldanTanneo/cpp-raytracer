
// From src/include
#include <hittable.hpp>
#include <materials/diffuse.hpp>
#include <utils.hpp>

bool Diffuse::scatter(const HitRecord & hit_record, Ray & ray_out, Colour & attenuation) const noexcept {
    ray_out.origin = hit_record.hit_point;
    ray_out.direction = hit_record.surface_normal + Vec3::random_in_unit_sphere();
    if (ray_out.direction.near_zero()) {
        ray_out.direction = hit_record.surface_normal;
    }
    attenuation *= albedo;
    return true;
}
