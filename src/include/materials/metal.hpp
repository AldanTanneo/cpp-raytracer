#ifndef METAL_HPP
#define METAL_HPP

#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

/* Metal material: perfectly reflects the incident ray and adds a random
fuzziness component */
class Metal : public Material {
private:
    /* The metallic colour */
    Colour albedo;
    /* Fuzziness parameter: distorts the reflection */
    double fuzziness;

public:
    /* Construct a new metal material */
    constexpr Metal(Colour albedo, double fuzziness = 0.0) noexcept
        : albedo(albedo), fuzziness(fuzziness) {}

    /* Virtual function override */
    virtual ScatterType scatter(const HitRecord & hit_record,
                                Ray & ray,
                                Colour & ray_colour) const noexcept override;
};

#endif
