#ifndef PLASTIC_HPP
#define PLASTIC_HPP

#include <cmath>
#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

// Plastic material: reflects and diffuses light
class Plastic : public Material {
private:
    // The colour of the plastic material
    Colour colour;
    // The roughness of the surface: distorts the reflection
    double roughness;

public:
    // Construct a new plastic material
    constexpr Plastic(Colour colour, double roughness = 0.0) noexcept
        : colour(colour), roughness(roughness) {}

    // Virtual function override
    virtual ScatterType
        scatter(const HitRecord & hit_record,
                const Ray & ray_in,
                ScatterRecord & scatter) const noexcept override;

    // Virtual function override
    virtual double
        scattering_pdf(const HitRecord & hit_record,
                       const Ray & scattered_ray) const noexcept override;
};

#endif
