#ifndef DIFFUSE_HPP
#define DIFFUSE_HPP

#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

/* Diffuse material: randomly scatters around the surface normal */
class Diffuse : public Material {
private:
    /* The colour of the diffuse material */
    Colour albedo;

public:
    constexpr Diffuse(Colour albedo) noexcept : albedo(albedo) {}

    virtual bool scatter(const HitRecord & hit_record,
                         Ray & ray,
                         Colour & ray_colour) const noexcept override;
};

#endif
