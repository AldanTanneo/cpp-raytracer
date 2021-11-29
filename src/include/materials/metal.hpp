#ifndef METAL_HPP
#define METAL_HPP

#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

/* Metal material: perfectly reflects the incident ray and adds a random
fuzziness component */
class Metal : public Material {
private:
    Colour albedo;
    double fuzziness;

public:
    constexpr Metal(Colour albedo, double fuzziness = 0.0) noexcept
        : albedo(albedo), fuzziness(fuzziness) {}

    virtual bool scatter(const HitRecord & hit_record,
                         Ray & ray,
                         Colour & ray_colour) const noexcept override;
};

#endif
