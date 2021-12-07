#ifndef DIFFUSE_HPP
#define DIFFUSE_HPP

#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

// Diffuse material: randomly scatters around the surface normal
class Diffuse : public Material {
private:
    // The colour of the diffuse material
    Colour albedo;

public:
    // Construct a new diffuse material
    constexpr Diffuse(Colour albedo) noexcept : albedo(albedo) {}

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
