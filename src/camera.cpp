#include <cmath>
#include <numeric>

// From src/include
#include <camera.hpp>
#include <vec3.hpp>

Camera::Camera(Point3 origin, Point3 look_at, Vec3 up_vector, double vertical_fov, AspectRatio aspect_ratio) : origin(origin) {
    if (vertical_fov < utils::EPSILON || vertical_fov > 180 - utils::EPSILON) {
        throw "Could not build camera: vertical fov must be an angle between 0° and 180°";
    }
    if (aspect_ratio.value() < utils::EPSILON) {
        throw "Could not build camera: aspect_ratio must be a positive value";
    }

    Vec3 direction = look_at - origin;
    double distance_to_focus = direction.norm();
    if (distance_to_focus < utils::EPSILON) {
        throw "Could not build camera: the origin and focus points must be different";
    }

    // Compute an orthonormal base of the screen space
    Vec3 x = direction / distance_to_focus;
    Vec3 z = x.cross(up_vector).unit_vector();
    Vec3 y = z.cross(x); // No need to normalize, it's a cross product between 2 unit vectors
    if (z.squared_norm() < utils::EPSILON) {
        throw "Could not build camera: the view direction and up vector cannot be parallel";
    }

    // Compute the height and width of the virtual screen
    double viewport_height = 2 * distance_to_focus * tan(utils::to_radians(vertical_fov) / 2);
    double viewport_width = viewport_height * aspect_ratio.value();

    // Final camera setup
    horizontal_vector = viewport_width * z;
    vertical_vector = viewport_height * y;
    origin_to_bottom_left_corner = direction - horizontal_vector / 2 - vertical_vector / 2;
}
