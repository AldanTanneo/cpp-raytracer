#ifndef EMISSIVE_HPP
#define EMISSIVE_HPP

#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

// Emissive material: always absorb the ray, multiply the ray colour according
// to the hit angle
class Emissive : public Material {
private:
    // The colour of the emissive material
    Colour colour;

public:
    // Constructs a new emissive material
    constexpr Emissive(Colour colour, double intensity = 2.0) noexcept
        : colour(intensity * colour) {}

    // Virtual function override
    virtual ScatterType
        scatter(const HitRecord & hit_record,
                const Ray & ray_in,
                ScatterRecord & scatter) const noexcept override;
};

// Perfect light source material: always absorb the ray (black body model)
class BlackBody : public Material {
private:
    // The colour of the emissive material
    Colour colour;

public:
    // Constructs a new emissive material
    constexpr BlackBody(Colour colour, double intensity = 2.0) noexcept
        : colour(intensity * colour) {}

    // Virtual function override
    virtual ScatterType
        scatter(const HitRecord & hit_record,
                const Ray & ray_in,
                ScatterRecord & scatter) const noexcept override;
};

#endif
