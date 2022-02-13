#include <camera.hpp>
#include <hittable.hpp>
#include <materials/dielectric.hpp>
#include <materials/diffuse.hpp>
#include <materials/emissive.hpp>
#include <materials/metal.hpp>
#include <utils/vec3.hpp>
#include <extern/json.hpp>
#include <fstream>
#include <memory>
#include <string>
#include <type_traits>
#include <utils/load_json.hpp>

using namespace nlohmann;
using namespace std;

Vec3 load_vec3(json j) { return Vec3(j.at(0), j.at(1), j.at(2)); }

Colour load_colour(json j) { return Colour(j.at(0), j.at(1), j.at(2)); }

AspectRatio load_aspect_ratio(json & j) {
    try {
        return AspectRatio(j.at(0), j.at(1));
    } catch (...) {
        return AspectRatio(j);
    }
}

Camera load_cam(json & j) {
    Point3 origin = load_vec3(j.at("origin"));
    Point3 look_at = load_vec3(j.at("look_at"));
    Vec3 up_vector = load_vec3(j.at("look_at"));
    double vertical_fov = j.at("vertical_fov");
    AspectRatio aspect_ratio = load_aspect_ratio(j.at("aspect_ratio"));
    return Camera(origin, look_at, up_vector, vertical_fov, aspect_ratio);
}

ImageInfo load_image_info(json & j) {
    size_t height = j.at("height");
    int max_bounces = j.at("max_bounces");
    int spp = j.at("spp");
    return ImageInfo{height, max_bounces, spp};
}

vector<GlobalIllumination> load_lights(json & j) {
    vector<GlobalIllumination> lights;
    Colour colour;
    string light_type_string;
    LightType light_type;
    Point3 position;

    for (auto it : j) {
        colour = load_colour(j.at("colour"));
        light_type_string = j.at("light_type").get<string>();
        if (light_type_string == "ambient") {
            light_type = Ambient;
            position = point3::ZEROS;

        } else if (light_type_string == "infinite") {
            light_type = Infinite;
            position = load_vec3(j.at("direction"));

        } else if (light_type_string == "point") {
            light_type = Point;
            position = load_vec3(j.at("position"));

        } else {
            throw "Invalid light type !";
        }
        lights.push_back(GlobalIllumination(colour, light_type, position));
    }
    return lights;
}

vector<unique_ptr<Material>> load_materials(json & j) {
    vector<unique_ptr<Material>> materials;
    Colour colour;
    string material_type;
    double generic_double;

    for (auto it : j) {
        colour = load_colour(j.at("colour"));
        material_type = j.at("material_type").get<string>();

        if (material_type == "lambertian") {
            materials.push_back(make_unique<Lambertian>(colour));

        } else if (material_type == "microfacet") {
            generic_double = j.at("sigma");
            materials.push_back(
                make_unique<Dielectric>(colour, generic_double));

        } else if (material_type == "dielectric") {
            generic_double = j.at("refraction index");
            materials.push_back(
                make_unique<Dielectric>(colour, generic_double));

        } else if (material_type == "metal") {
            generic_double = j.at("fuzziness");
            materials.push_back(make_unique<Metal>(colour, generic_double));

            // TODO from here

        } else if (material_type == "plastic") {
            colour = load_colour(j.at("colour"));
            materials.push_back(make_unique<Plastic>(colour));

        } else if (material_type == "emissive") {
            generic_double = j.at("intensity");
            materials.push_back(make_unique<Emissive>(colour, generic_double));

        } else if (material_type == "black body") {
            generic_double = j.at("intensity");
            materials.push_back(make_unique<BlackBody>(colour, generic_double));

        } else {
            throw "Invalid Material type !";
        }
    }
    return materials;
}

vector<unique_ptr<Hittable>>
load_objects(json & j, vector<unique_ptr<Material>> & materials) {}

Params load_params(const char * file_name) {
    ifstream file(file_name, ios_base::in | ios_base::binary);
    file.open(file_name);
    if (!file) {
        throw "Could not load json file: could not open file!";
    }

    json j;
    j = json::parse(file);

    Camera cam = load_cam(j.at("camera"));
    ImageInfo info = load_image_info(j.at("image"));
    vector<GlobalIllumination> global_lights = load_lights(j.at("lights"));
    vector<unique_ptr<Material>> materials = load_materials(j.at("materials"));
    vector<unique_ptr<Hittable>> objects =
        load_objects(j.at("objects"), materials);

    file.close();

    return Params{cam, info, global_lights, materials, objects};
}