#ifndef DIFFUSE_HPP
#define DIFFUSE_HPP

#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

// Lambertian material: randomly scatters around the surface normal
class Lambertian : public Material {
private:
    // The colour of the diffuse material
    const Colour albedo;

public:
    // Construct a new diffuse material
    constexpr Lambertian(Colour albedo) noexcept : albedo(albedo) {}

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

// Oren-Nayar microfacet model
// (https://pbr-book.org/3ed-2018/Reflection_Models/Microfacet_Models#OrenndashNayarDiffuseReflection)
class MicroFacet : public Material {
private:
    // The colour of the diffuse material
    const Colour albedo;
    // Material parameters, dependant on the roughness of the material
    const double A, B;

    // Helper function to construct the A parameter
    constexpr static double construct_A(double sigma) noexcept {
        const double variance = sigma * sigma;
        return 1.0 - variance / (2.0 * (variance + 0.33));
    }

    // Helper function to construct the B parameter
    constexpr static double construct_B(double sigma) noexcept {
        const double variance = sigma * sigma;
        return 0.45 * variance / (variance + 0.09);
    }

public:
    // Construct a new MicroFacet material. "sigma" is the standard deviation of
    // the microfacet angle, in degrees
    constexpr MicroFacet(Colour albedo, const double sigma) noexcept
        : albedo(albedo), A(construct_A(utils::to_radians(sigma))),
          B(construct_B(utils::to_radians(sigma))) {}

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
