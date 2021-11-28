#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cmath>
#include <numeric>

// From src/include
#include <hittable.hpp>
#include <ray.hpp>
#include <utils/vec3.hpp>

/* Aspect ratio wrapper class */
struct AspectRatio {
private:
    /* The wrapped aspect ratio */
    double aspect_ratio;

public:
    /* Construct an aspect ratio from two integers */
    constexpr AspectRatio(unsigned int width, unsigned int height)
        : aspect_ratio(double(width) / double(height)) {
        if (aspect_ratio < utils::EPSILON || aspect_ratio == utils::INF ||
            aspect_ratio != aspect_ratio) {
            throw "Could not construct AspectRatio: value must be positive";
        }
    }
    /* Construct an aspect ratio from a double */
    constexpr AspectRatio(double value) : aspect_ratio(value) {
        if (aspect_ratio < utils::EPSILON || aspect_ratio == utils::INF ||
            aspect_ratio != aspect_ratio) {
            throw "Could not construct AspectRatio: value must be positive";
        }
    }

    /* Get the inner value */
    constexpr long double value() const noexcept { return aspect_ratio; }
};

/* Main camera class */
class Camera {
private:
    /* Origin point */
    Point3 origin;
    /* Vector from the origin to the bottom left corner of the screen */
    Vec3 origin_to_bottom_left_corner;
    /* Vertical vector of the screen space */
    Vec3 vertical_vector;
    /* Horizontal vector of the screen space */
    Vec3 horizontal_vector;

public:
    /* Construct a camera */
    Camera(Point3 origin,
           Point3 look_at,
           Vec3 up_vector,
           double vertical_fov,
           AspectRatio aspect_ratio);

    /* Get a ray from normalized coordinates in the virtual screen space */
    constexpr Ray get_ray(double u, double v) const noexcept {
        return Ray(origin, origin_to_bottom_left_corner +
                               u * horizontal_vector + v * vertical_vector);
    }

    template <const uint32_t max_rec>
    inline Colour cast_ray(const Hittable & world,
                           Colour background_colour,
                           double u,
                           double v) const noexcept {
        Ray r = get_ray(u, v);
        Colour ray_colour = background_colour;
        HitRecord h;
        uint32_t iter = 0;
        while (iter != max_rec) {
            if (world.hit(r, utils::EPSILON, utils::INF, h)) {
                iter++;
                h.material->scatter(h, r, ray_colour);
            } else {
                break;
            }
        }
        return ray_colour;
    }
};

#endif
