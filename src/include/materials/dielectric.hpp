#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include <cmath>
#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

/* Dielectric material: partially refracts light */
class Dielectric : public Material {
private:
    Colour colour;
    double refraction_index;

public:
    constexpr Dielectric(Colour colour, double refraction_index = 0.0) noexcept
        : colour(colour), refraction_index(refraction_index) {}

    virtual bool scatter(const HitRecord & hit_record,
                         Ray & ray,
                         Colour & ray_colour) const noexcept override;

private:
    static inline double reflectance(double cos_theta,
                                     double refraction_ratio) {
        double r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cos_theta), 5);
    }
};

#endif
