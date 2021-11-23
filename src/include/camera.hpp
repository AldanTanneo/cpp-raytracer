#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <numeric>

// From src/include
#include <ray.hpp>
#include <vec3.hpp>

/* Aspect ratio wrapper class */
struct AspectRatio {
  private:
    /* The wrapped aspect ratio */
    long double aspect_ratio;

  public:
    /* Construct an aspect ratio from two integer */
    constexpr AspectRatio(unsigned int width, unsigned int height) noexcept : aspect_ratio((long double)width / (long double)height) {}
    /* Construct an aspect ratio from a long double */
    constexpr AspectRatio(long double value) noexcept : aspect_ratio(value) {}

    /* Get the inner value */
    constexpr long double value() noexcept { return aspect_ratio; }
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
    Camera(Point3 origin, Point3 look_at, Vec3 up_vector, double vertical_fov, AspectRatio aspect_ratio);

    /* Get a ray from normalized coordinates in the virtual screen space */
    constexpr Ray get_ray(double u, double v) const noexcept {
        return Ray(origin, origin_to_bottom_left_corner + u * horizontal_vector + v * vertical_vector);
    }
};

#endif
