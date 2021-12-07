#include <hittable.hpp>
#include <materials/metal.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

Metal::ScatterType Metal::scatter(const HitRecord & hit_record,
                                  const Ray & ray_in,
                                  ScatterRecord & scatter) const noexcept {
    scatter.is_specular = true;
    scatter.attenuation = albedo;

    double normal_component = ray_in.direction.dot(hit_record.surface_normal);
    scatter.specular_direction =
        ray_in.direction
        - normal_component
              * (fuzziness * Vec3::random_in_unit_sphere()
                 + 2.0 * hit_record.surface_normal);

    return ScatterType::Bounce;
}
