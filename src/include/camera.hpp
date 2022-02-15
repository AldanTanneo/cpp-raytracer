#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cmath>
#include <numeric>

// From src/include
#include <hittable.hpp>
#include <ray.hpp>
#include <utils/orthonormal_bases.hpp>
#include <utils/vec3.hpp>

// Aspect ratio wrapper class
struct AspectRatio {
private:
    // The wrapped aspect ratio
    double aspect_ratio;

public:
    // Default constructor
    constexpr AspectRatio() : aspect_ratio(1.0) {}

    // Construct an aspect ratio from two integers
    constexpr AspectRatio(unsigned int width, unsigned int height)
        : aspect_ratio(double(width) / double(height)) {
        if (aspect_ratio < utils::EPSILON || aspect_ratio == utils::INF
            || aspect_ratio != aspect_ratio) {
            throw "Could not construct AspectRatio: value must be positive";
        }
    }
    // Construct an aspect ratio from a double
    constexpr AspectRatio(double value) : aspect_ratio(value) {
        if (aspect_ratio < utils::EPSILON || aspect_ratio == utils::INF
            || aspect_ratio != aspect_ratio) {
            throw "Could not construct AspectRatio: value must be positive";
        }
    }

    // Get the inner value
    constexpr double value() const noexcept { return aspect_ratio; }
};

// Light type
enum LightType {
    // Ambient lights: illuminate from everywhere at once
    Ambient = 0,
    // Infinite lights: illuminate from a fixed direction in space
    Infinite = 1,
    // Point lights: illuminate from a fixed point in space
    Point = 2
};

// Global light
struct GlobalIllumination {
    // The global light type
    LightType type;
    // For point lights, the position of the light; for infinite lights, the
    // direction of the light
    Point3 position;
    // The colour of the light
    Colour colour;

    // Construct a global light instance
    constexpr GlobalIllumination(const Colour colour,
                                 const LightType type = LightType::Ambient,
                                 const Point3 position = point3::ZEROS)
        : type(type),
          position(type == Infinite ? position.unit_vector() : position),
          colour(colour) {}
};

// Main camera class
class Camera {
private:
    // Origin point
    Point3 origin;
    // Vector from the origin to the bottom left corner of the screen
    Vec3 origin_to_bottom_left_corner;
    // Vertical vector of the screen space
    Vec3 vertical_vector;
    // Horizontal vector of the screen space
    Vec3 horizontal_vector;
    // Orthonormal base of the camera
    Onb base;

public:
    Camera() = default;

    // Construct a camera
    Camera(Point3 origin,
           Point3 look_at,
           Vec3 up_vector,
           double vertical_fov,
           const AspectRatio & aspect_ratio);

    // Get a ray from normalized coordinates in the virtual screen space
    constexpr Ray get_ray(double u, double v) const noexcept {
        return Ray(origin, origin_to_bottom_left_corner + u * horizontal_vector
                               + v * vertical_vector);
    }

    // Cast a ray into the world with the given parameters
    // and at the given screen space coordinates
    Colour cast_ray(const Hittable & world,
                    const std::vector<GlobalIllumination> & global_lights,
                    const Hittable & sampled_object,
                    const uint32_t max_bounces,
                    double u,
                    double v) const noexcept;
};

#endif
