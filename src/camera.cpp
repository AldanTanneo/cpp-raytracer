#include <cmath>

// From src/include
#include <camera.hpp>
#include <vec3.hpp>

Camera::Camera(Point3 origin, Point3 look_at, Vec3 up_vector, double vertical_fov, double aspect_ratio) noexcept : origin(origin) {
    Vec3 direction = look_at - origin;
    double distance_to_focus = direction.norm();
    // Compute an orthonormal base of the screen space
    Vec3 x = direction / distance_to_focus;
    Vec3 z = x.cross(up_vector).unit_vector();
    Vec3 y = z.cross(x); // No need to normalize, it's a cross product between 2 unit vectors

    // Compute the height and width of the virtual screen
    double viewport_height = 2 * distance_to_focus * tan(vertical_fov / 2);
    double viewport_width = viewport_height * aspect_ratio;

    // Final camera setup
    horizontal_vector = viewport_width * z;
    vertical_vector = viewport_height * y;
    origin_to_bottom_left_corner = look_at - horizontal_vector / 2 - vertical_vector / 2 - origin;
}
