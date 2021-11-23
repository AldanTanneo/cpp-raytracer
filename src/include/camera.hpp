#ifndef CAMERA_HPP
#define CAMERA_HPP

// From src/include
#include <ray.hpp>
#include <vec3.hpp>

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
    Camera(Point3 origin, Point3 look_at, Vec3 up_vector, double vertical_fov, double aspect_ratio) noexcept;

    /* Get a ray from normalized coordinates in the virtual screen space */
    constexpr Ray get_ray(double u, double v) const noexcept {
        return Ray(origin, origin_to_bottom_left_corner + u * horizontal_vector + v * vertical_vector);
    }
};

#endif
