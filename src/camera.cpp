#include <cmath>
#include <numeric>

// From src/include
#include <camera.hpp>
#include <utils/vec3.hpp>

Camera::Camera(Point3 origin,
               Point3 look_at,
               Vec3 up_vector,
               double vertical_fov,
               const AspectRatio & aspect_ratio)
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
    base = Onb::from_base_vectors(x, y, z);
}

using namespace std;

Colour Camera::cast_ray(const Hittable & world,
                        const vector<GlobalIllumination> & global_lights,
                        const Hittable & sampled_object,
                        const uint32_t max_bounces,
                        double u,
                        double v) const noexcept {
    Ray ray = get_ray(u, v);
    Colour ray_colour = colour::WHITE;
    HitRecord hit_record;
    ScatterRecord scatter;
    uint32_t iter = 0;
    double pdf_coeff = 1.0;
    double pdf_value = 1.0;
    for (iter = 0; iter < max_bounces; ++iter) {
        if (!world.hit(ray, utils::EPSILON, utils::INF, hit_record)) {
            break;
        }

// Display normals
#if 0
        return Colour(0.5 + 0.5 * record.surface_normal);
#endif

        if (hit_record.scatter(ray, scatter) != Material::ScatterType::Bounce) {
            return (fabs(pdf_value) < utils::EPSILON
                        ? colour::BLACK
                        : pdf_coeff * ray_colour * scatter.attenuation
                              / pdf_value);
        }

        ray_colour *= scatter.attenuation;
        ray.origin = hit_record.hit_point;
        if (scatter.is_specular) {
            ray.direction = scatter.specular_direction;
        } else {
            const HittablePdf light_pdf(sampled_object, hit_record.hit_point);
            const MixturePdf mixture_pdf((const Pdf &)*scatter.pdf.get(),
                                         light_pdf);
            const Pdf & pdf = sampled_object.is_samplable()
                                  ? mixture_pdf
                                  : (const Pdf &)*scatter.pdf.get();
            ray_colour *= scatter.attenuation;
            ray.direction = pdf.generate();
            if (ray.direction.dot(hit_record.surface_normal) < utils::EPSILON) {
                pdf_value = 0.0;
                break;
            } else {
                pdf_value *= pdf.value(ray.direction);
            }

            pdf_coeff *=
                hit_record.material.get().scattering_pdf(hit_record, ray);
        }
    }

    Colour lights_contribution;
    for (const GlobalIllumination & light : global_lights) {
        double light_coeff = 1.0;
        Vec3 light_direction = light.position;
        if (light.type == LightType::Point) {
            light_direction = light.position - ray.origin;
            light_direction /= light_direction.squared_norm();
        }
        if (light.type != LightType::Ambient) {
            light_coeff =
                (iter
                     ? fmax(hit_record.surface_normal.dot(light_direction), 0.0)
                     : light.type != LightType::Point)
                * fmax(ray.direction.unit_vector().dot(light_direction), 0.0);
        }
        lights_contribution += light_coeff * light.colour;
    }

    return (fabs(pdf_value) < utils::EPSILON)
               ? colour::BLACK
               : pdf_coeff * ray_colour * lights_contribution / pdf_value;
}
