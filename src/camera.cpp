#include <cmath>
#include <numeric>

// From src/include
#include <camera.hpp>
#include <utils/vec3.hpp>

Camera::Camera(Point3 origin,
               Point3 look_at,
               Vec3 up_vector,
               double vertical_fov,
               AspectRatio aspect_ratio)
    : origin(origin) {
    if (vertical_fov < utils::EPSILON || vertical_fov > 180 - utils::EPSILON) {
        throw "Could not build camera: vertical fov must be an angle between "
              "0° and 180°";
    }

    Vec3 direction = look_at - origin;
    if (direction.squared_norm() < utils::EPSILON) {
        throw "Could not build camera: the origin and focus points must be "
              "different";
    }

    // Compute an orthonormal base of the screen space
    Vec3 x = direction.unit_vector();
    Vec3 z = x.cross(up_vector).unit_vector();
    Vec3 y = z.cross(
        x); // No need to normalize, it's a cross product between 2 unit vectors
    if (z.squared_norm() < utils::EPSILON) {
        throw "Could not build camera: the view direction and up vector cannot "
              "be parallel";
    }

    // Compute the height and width of the virtual screen
    double viewport_height = 2 * tan(utils::to_radians(vertical_fov) / 2);
    double viewport_width = viewport_height * aspect_ratio.value();

    // Final camera setup
    horizontal_vector = viewport_width * z;
    vertical_vector = viewport_height * y;
    origin_to_bottom_left_corner =
        x - horizontal_vector / 2 - vertical_vector / 2;
}

using namespace std;

Colour Camera::cast_ray(const Hittable & world,
                        const vector<GlobalIllumination> & lights,
                        const uint32_t max_bounces,
                        double u,
                        double v) const noexcept {
    Ray ray = get_ray(u, v);
    Colour ray_colour = colour::WHITE;
    HitRecord record;
    uint32_t iter;
    for (iter = 0; iter < max_bounces; ++iter) {
        if (!world.hit(ray, utils::EPSILON, utils::INF, record)) {
            break;
        }

        // Display normals
        // return Colour(0.5 + 0.5 * record.surface_normal);

        if (record.scatter(ray, ray_colour) != Material::ScatterType::Bounce) {
            return ray_colour;
        }
    }

    Colour exit_colour;
    for (const GlobalIllumination & light : lights) {
        double light_coeff = 1.0;
        Vec3 light_direction = light.position;
        if (light.type == LightType::Point) {
            light_direction = light.position - ray.origin;
            light_direction /= light_direction.squared_norm();
        }
        if (light.type != LightType::Ambient) {
            light_coeff =
                (iter ? fmax(record.surface_normal.dot(light_direction), 0.0)
                      : light.type != LightType::Point)
                * fmax(ray.direction.unit_vector().dot(light_direction), 0.0);
        }
        exit_colour += ray_colour * light_coeff * light.colour;
    }

    return exit_colour;
}
