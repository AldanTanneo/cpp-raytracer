#include <hittable.hpp>
#include <materials/diffuse.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

bool Diffuse::scatter(const HitRecord & hit_record,
                      Ray & ray,
                      Colour & ray_colour) const noexcept {
    ray.origin = hit_record.hit_point;
    ray.direction = hit_record.surface_normal + Vec3::random_in_unit_sphere();
    if (ray.direction.near_zero()) {
        ray.direction = hit_record.surface_normal;
    }
    ray_colour *= albedo;
    return true;
}
