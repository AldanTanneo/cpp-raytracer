#include <hittable.hpp>
#include <materials/emissive.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

Emissive::ScatterType Emissive::scatter(const HitRecord & hit_record,
                                        Ray & ray,
                                        Colour & ray_colour) const noexcept {
    ray_colour *= colour;
    return ScatterType::Emit;
}
