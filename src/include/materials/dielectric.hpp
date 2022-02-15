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
    constexpr Dielectric(Colour colour, double refraction_index = 1.0) noexcept
        : colour(colour), refraction_index(refraction_index) {}

    // Virtual function override
    virtual ScatterType
        scatter(const HitRecord & hit_record,
                const Ray & ray_in,
                ScatterRecord & scatter) const noexcept override;
};

// Translucent material: partially refracts light
class Translucent : public Material {
private:
    // The colour of the dielectric material
    Colour colour;
    // The refraction index of the medium
    double refraction_index;
    // The random proportion in refraction
    double fuzziness;
    // The random proportion in reflection
    double surface_fuzziness;

public:
    // Construct a new dielectric material
    constexpr Translucent(Colour colour,
                          double refraction_index = 1.0,
                          double fuzziness = 0.0,
                          double surface_fuzziness = 0.0) noexcept
        : colour(colour), refraction_index(refraction_index),
          fuzziness(fuzziness), surface_fuzziness(surface_fuzziness) {}

    // Virtual function override
    virtual ScatterType
        scatter(const HitRecord & hit_record,
                const Ray & ray_in,
                ScatterRecord & scatter) const noexcept override;
};

#endif
