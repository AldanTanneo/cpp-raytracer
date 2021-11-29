#include <hittable.hpp>
#include <materials/metal.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

bool Metal::scatter(const HitRecord & hit_record,
                    Ray & ray,
                    Colour & ray_colour) const noexcept {
    ray.origin = hit_record.hit_point;
    double normal_component = ray.direction.dot(hit_record.surface_normal);
    ray.direction = ray.direction
                    + normal_component
                          * (fuzziness * Vec3::random_in_unit_sphere()
                             - 2.0 * hit_record.surface_normal);
    ray_colour *= albedo;
    return true;
}
