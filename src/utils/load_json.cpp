#include <fstream>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

// From src/include
#include <camera.hpp>
#include <extern/json.hpp>
#include <hittable.hpp>
#include <materials/dielectric.hpp>
#include <materials/diffuse.hpp>
#include <materials/emissive.hpp>
#include <materials/metal.hpp>
#include <materials/plastic.hpp>
#include <objects/cylinder.hpp>
#include <objects/parallelogram.hpp>
#include <objects/sphere.hpp>
#include <objects/triangle.hpp>
#include <utils/load_json.hpp>
#include <utils/vec3.hpp>

using namespace nlohmann;
using namespace std;

static inline Vec3 load_vec3(const json & j) {
    return Vec3(j.at(0).get<double>(), j.at(1).get<double>(),
                j.at(2).get<double>());
}

static inline Colour load_colour(const json & j) {
    return Colour(j.at(0).get<double>(), j.at(1).get<double>(),
                  j.at(2).get<double>());
}

static inline AspectRatio load_aspect_ratio(const json & j) {
    if (j.is_array()) {
        return AspectRatio(j.at(0).get<unsigned int>(),
                           j.at(1).get<unsigned int>());
    } else {
        return AspectRatio(j.get<double>());
    }
}

static Camera load_cam(const json & j) {
    Point3 origin = load_vec3(j.at("origin"));
    Point3 look_at = load_vec3(j.at("look_at"));
    Vec3 up_vector = load_vec3(j.at("look_at"));
    double vertical_fov = j.at("vertical_fov");
    AspectRatio aspect_ratio = load_aspect_ratio(j.at("aspect_ratio"));
    return Camera(origin, look_at, up_vector, vertical_fov, aspect_ratio);
}

static ImageInfo load_image_info(const json & j) {
    size_t height = j.at("height");
    int max_bounces = j.at("max_bounces");
    int spp = j.at("spp");
    return ImageInfo { height, max_bounces, spp };
}

static vector<GlobalIllumination> load_lights(const json & j) {
    vector<GlobalIllumination> lights;
    Colour colour;
    string light_type_string;
    LightType light_type;
    Point3 position;
    if (!j.is_array()) {
        throw "Invalid JSON: global_lights must be an array of "
              "GlobalIllumination objects.";
    }

    for (const json & light : j) {
        if (!light.is_object()) {
            throw "Invalid JSON: global_lights must be an array of "
                  "GlobalIllumination objects.";
        }

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
            throw "Invalid light type!";
        }

        lights.push_back(GlobalIllumination(colour, light_type, position));
    }
    return lights;
}

static unordered_map<string, unique_ptr<Material>>
    load_materials(const json & j) {
    if (!j.is_object()) {
        throw "Invalid JSON: global_lights must be a map of Material "
              "objects.";
    }

    unordered_map<string, unique_ptr<Material>> materials;
    Colour colour;
    string material_type;
    double generic_double;

    for (const auto & [key, mat] : j.items()) {
        if (!mat.is_object()) {
            throw "Invalid JSON: global_lights must be a map of Material "
                  "objects.";
        }

        colour = load_colour(mat.at("colour"));
        material_type = mat.at("material_type").get<string>();

        if (material_type == "lambertian") {
            materials.insert_or_assign(key, make_unique<Lambertian>(colour));

        } else if (material_type == "microfacet") {
            generic_double = mat.at("sigma");
            materials.insert_or_assign(
                key, make_unique<Dielectric>(colour, generic_double));

        } else if (material_type == "dielectric") {
            generic_double = mat.at("refraction_index");
            materials.insert_or_assign(
                key, make_unique<Dielectric>(colour, generic_double));

        } else if (material_type == "translucent") {
            double refraction_index = mat.at("refraction_index").get<double>();
            double fuzziness = mat.at("fuzziness").get<double>();
            double surface_fuzziness = fuzziness;
            if (mat.contains("surface_fuzziness")) {
                surface_fuzziness = mat["surface_fuzziness"].get<double>();
            }

            materials.insert_or_assign(
                key, make_unique<Translucent>(colour, refraction_index,
                                              fuzziness, surface_fuzziness));

        } else if (material_type == "metal") {
            generic_double = mat.at("fuzziness");
            materials.insert_or_assign(
                key, make_unique<Metal>(colour, generic_double));

        } else if (material_type == "plastic") {
            generic_double = mat.at("roughness");
            materials.insert_or_assign(
                key, make_unique<Plastic>(colour, generic_double));

        } else if (material_type == "emissive") {
            generic_double = mat.at("intensity");
            materials.insert_or_assign(
                key, make_unique<Emissive>(colour, generic_double));

        } else if (material_type == "black_body") {
            generic_double = mat.at("intensity");
            materials.insert_or_assign(
                key, make_unique<BlackBody>(colour, generic_double));

        } else {
            throw "Invalid material type!";
        }
    }
    return materials;
}

static pair<vector<unique_ptr<Hittable>>, vector<size_t>> load_objects(
    const json & j,
    const unordered_map<string, unique_ptr<Material>> & materials) {
    if (!j.is_array()) {
        throw "Invalid JSON: objects must be an array of Hittable objects.";
    }

    vector<unique_ptr<Hittable>> objects;
    vector<size_t> sampled_objects;
    string object_type;
    string material_name;
    size_t index = 0;

    for (const json & obj : j) {
        if (!obj.is_object()) {
            throw "Invalid JSON: objects must be an array of Hittable objects.";
        }
        ++index;

        object_type = obj.at("object_type").get<string>();
        material_name = obj.at("material").get<string>();
        if (obj.contains("sampled") && obj.at("sampled").get<bool>()) {
            sampled_objects.push_back(index);
        }

        if (object_type == "sphere") {
            Vec3 center = load_vec3(obj.at("center"));
            double radius = obj.at("radius").get<double>();

            objects.push_back(make_unique<Sphere>(
                center, radius,
                (const Material &)*materials.at(material_name)));

        } else if (object_type == "triangle") {
            const json & vertices = obj.at("vertices");
            Vec3 p0 = load_vec3(vertices.at(0));
            Vec3 p1 = load_vec3(vertices.at(1));
            Vec3 p2 = load_vec3(vertices.at(2));

            objects.push_back(make_unique<Triangle>(
                p0, p1, p2, (const Material &)*materials.at(material_name)));

        } else if (object_type == "parallelogram") {
            const json & vertices = obj.at("vertices");
            Vec3 p0 = load_vec3(vertices.at(0));
            Vec3 p1 = load_vec3(vertices.at(1));
            Vec3 p2 = load_vec3(vertices.at(2));

            objects.push_back(make_unique<Parallelogram>(
                p0, p1, p2, (const Material &)*materials.at(material_name)));

        } else if (object_type == "cylinder") {
            Point3 base = load_vec3(obj.at("base"));
            Vec3 axis = load_vec3(obj.at("axis"));
            double radius = obj.at("radius").get<double>();
            double height = obj.at("height").get<double>();

            objects.push_back(make_unique<Cylinder>(
                base, axis, radius, height,
                (const Material &)*materials.at(material_name)));

        } else {
            throw "Invalid object type!";
        }
    }

    return make_pair(objects, sampled_objects);
}

Params Params::load_params(const char * file_name) {
    ifstream file(file_name, ios_base::in | ios_base::binary);
    file.open(file_name);
    if (!file) {
        throw "Could not load json file: could not open file!";
    }

    const json j = json::parse(file);

    Camera cam = load_cam(j.at("camera"));
    ImageInfo info = load_image_info(j.at("image"));
    vector<GlobalIllumination> global_lights = load_lights(j.at("lights"));
    unordered_map<string, unique_ptr<Material>> materials =
        load_materials(j.at("materials"));
    auto [objects, sampled_objects] = load_objects(j.at("objects"), materials);

    file.close();

    return Params { cam,       info,    global_lights,
                    materials, objects, sampled_objects };
}
