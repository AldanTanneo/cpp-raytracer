#ifndef DIFFUSE_HPP
#define DIFFUSE_HPP

#include <hittable.hpp>
#include <utils.hpp>

/* Diffuse material: randomly scatters around the surface normal */
class Diffuse : public Material {
private:
    Colour albedo;

public:
    constexpr Diffuse(Colour albedo) noexcept : albedo(albedo) {}

    virtual bool scatter(const HitRecord & hit_record,
                         Ray & ray_out,
                         Colour & ray_colour) const noexcept override;
};

#endif
