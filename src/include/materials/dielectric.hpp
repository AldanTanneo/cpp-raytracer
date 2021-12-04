#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include <cmath>
#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

// Dielectric material: partially refracts light
class Dielectric : public Material {
private:
    // The colour of the dielectric material
    Colour colour;
    // The refraction index of the medium
    double refraction_index;

public:
    // Construct a new dielectric material
    constexpr Dielectric(Colour colour, double refraction_index = 0.0) noexcept
        : colour(colour), refraction_index(refraction_index) {}

    // Virtual function override
    virtual ScatterType scatter(const HitRecord & hit_record,
                                Ray & ray,
                                Colour & ray_colour) const noexcept override;
};

#endif
